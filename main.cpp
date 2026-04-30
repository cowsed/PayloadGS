// #include <QApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QFileSystemWatcher>
#include <QPalette>
#include <QtConcurrentRun>

#include <QFile>
#include <QIcon>
#include <QJsonDocument>
#include <QOpenGLContext>
#include <QQmlContext>
#include <QSurfaceFormat>
#include "datasource.h"
#include <qquickview.h>

#include "imagedataholder.h"
#include "librarian.h"
#include "packets_p2g.h"
#include "radiopacketparser.h"
#include "telemetrylogholder.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    printf("TPE: %d %s\n", type, qPrintable(msg));
}

int main(int argc, char *argv[])
{
    FlightHeartbeatStats fhstats{
        .state = {
            .phase = FlightPhase_ExpectingLaunch,
            .status_bits = 0b1000000001,
        },
        .latitude = 43.082624,
        .longitude =  -77.669193,
        .altitude = 65000,
        .s_since_boost = 15,
        .battery_mV = 11100,
        .radio_temp = 40,
    };
    Telemetry telemfh{
        .telem_type = TelemetryType_FlightHeartbeat,
        .flight_heartbeat_stats = fhstats,
    };
    Telemetry telem{.telem_type = TelemetryType_Actuators,
                    .actuators = {
                        .arms = {
                          .shoulder_yaw = 15,
                          .shoulder_pitch = 0,
                          .elbow_pitch = 80,
                          .wrist_pitch = 30,
                    },
                        .servo1 = 0,
                        .servo2 = 1,
                        .servo3 = 13,
                        .servo4 = 240,
                    },
    };
    CommandResponse cmd_resp = {.cmd = Command_TelemetryRequest, .telemetry = telemfh};
    uint8_t buf[256] = {0XAA};
    struct P2GLinkHeader lh{.packet_type = P2GPacketType_CommandResponse,
                            .expected_packets_before_response = 3};
    int len = pack_p2g_link_header(&lh, buf);
    len += pack_command_response(&cmd_resp, buf + len);
    QByteArray arr{(char *) buf, len};
    printf("Length: %d\n", len);
    printf("buf: %s\n", qPrintable(arr.toBase64()));

    qInstallMessageHandler(myMessageOutput);

    QApplication app(argc, argv);
    QApplication::setDesktopFileName("PayloadGS");

    QIcon icon{":/assets/images/window_icon.png"};
    if (icon.isNull()) {
        qCritical("Couldnt load window icon");
    }

    app.setWindowIcon(icon);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/");

    QString working_dir = "/home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir";
    size_t flight_id = 0;

    QString flight_dir = QString("%1/%2").arg(working_dir).arg(flight_id);
    engine.rootContext()->setContextProperty("flight_dir", flight_dir);

    QQuickView viewer;

    TelemetryLogHolder *holder
        = engine.singletonInstance<TelemetryLogHolder *>("PayloadGS", "TelemetryLogHolder");

    RadioPacketParser *radio_parser
        = engine.singletonInstance<RadioPacketParser *>("PayloadGS", "RadioPacketParser");

    ImageDataHolder *img_holder = engine.singletonInstance<ImageDataHolder *>("PayloadGS",
                                                                              "ImageDataHolder");
    img_holder->setFlightDir(flight_dir);
    img_holder->rescanCount();

    Librarian *lib = engine.singletonInstance<Librarian *>("PayloadGS", "Librarian");
    lib->GatherRequestsFromDisk(img_holder);
    // lib->DumpInfo();

    qDebug("Have %zu requests", lib->NumRequests());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::imageDataReceived,
                     img_holder,
                     &ImageDataHolder::ImageDataReceived,
                     Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::payloadGPSUpdated,
                     holder,
                     &TelemetryLogHolder::newPayloadPosition,
                     Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::batteryUpdated,
                     holder,
                     &TelemetryLogHolder::newBatteryInformation,
                     Qt::QueuedConnection);

    engine.loadFromModule("PayloadGS", "Main");

    return app.exec();
}
