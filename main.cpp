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
#include "logprovider.h"
#include "outlawparser.h"
#include "radiopacketparser.h"
#include "telemetrylogholder.h"

#include <QSettings>

LogProvider *qt_logger{nullptr};
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (qt_logger) {
        qt_logger->appendLog(qFormatLogMessage(type, context, msg));
        printf("%s\n", qPrintable(qFormatLogMessage(type, context, msg)));
    } else {
        printf("%s\n", qPrintable(qFormatLogMessage(type, context, msg)));
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QApplication app(argc, argv);
    QApplication::setDesktopFileName("PayloadGS");
    app.setOrganizationName("RIT Launch Initiative");
    app.setOrganizationDomain("launchinitiative@rit.edu");
    app.setApplicationName("PayloadGS");
    QSettings settings;

    QIcon icon{":/assets/images/window_icon.png"};
    if (icon.isNull()) {
        qCritical("Couldnt load window icon");
    }

    app.setWindowIcon(icon);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/");

    QString default_flight_dir = "/home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir/1";
    QString flight_dir = settings.value("flight_dir", default_flight_dir).toString();
    engine.rootContext()->setContextProperty("flight_dir", flight_dir);

    QQuickView viewer;

    TelemetryLogHolder *holder
        = engine.singletonInstance<TelemetryLogHolder *>("PayloadGS", "TelemetryLogHolder");

    LogProvider *logger = engine.singletonInstance<LogProvider *>("PayloadGS", "LogProvider");
    qt_logger = logger;

    OutlawParser *outlaw_parser = engine.singletonInstance<OutlawParser *>("PayloadGS",
                                                                           "OutlawParser");

    RadioPacketParser *radio_parser
        = engine.singletonInstance<RadioPacketParser *>("PayloadGS", "RadioPacketParser");

    ImageDataHolder *img_holder = engine.singletonInstance<ImageDataHolder *>("PayloadGS",
                                                                              "ImageDataHolder");
    img_holder->setFlightDir(flight_dir);
    img_holder->rescanCount();

    QString packetLogFilename = flight_dir + "/rxed_packets.log";
    qInfo("Opening %s for packet log", qPrintable(packetLogFilename));
    QFile *packet_log = new QFile{packetLogFilename};
    packet_log->setParent(&app);
    if (!packet_log->open(QIODevice::Append | QIODevice::Text)) {
        qWarning("Failed to open packet log for writing");
    }

    Librarian *lib = engine.singletonInstance<Librarian *>("PayloadGS", "Librarian");

    qDebug("Have %zu requests", lib->NumRequests());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            qDebug("Clsoing");
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::imageDataReceived,
                     img_holder,
                     &ImageDataHolder::ImageDataReceived,
                     Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::imageDataReceived,
                     lib,
                     &Librarian::ImageDataReceived,
                     Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::ImageMetadataReceived,
                     img_holder,
                     &ImageDataHolder::ImageMetadataReceived,
                     Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::numImagesIncreased,
                     img_holder,
                     &ImageDataHolder::newImageAvailable,
                     Qt::QueuedConnection);

    // QObject::connect(radio_parser,
    //                  &RadioPacketParser::ImageMetadataReceived,
    //                  lib,
    //                  &Librarian::ImageMetadataReceived,
    //                  Qt::QueuedConnection);

    QObject::connect(
        radio_parser,
        &RadioPacketParser::packetReceivedFromRadio,
        &app,
        [&packet_log](QDateTime time, int snr, int rssi, QByteArray arr) {
            if (!packet_log->isOpen()) {
                return;
            }
            auto line = QString{"%1,%2,%3,%4\n"}
                            .arg(time.toString())
                            .arg(snr)
                            .arg(rssi)
                            .arg(arr.toBase64());
            packet_log->write(line.toUtf8());
        },
        Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::payloadGPSUpdated,
                     holder,
                     &TelemetryLogHolder::newPayloadPosition,
                     Qt::QueuedConnection);

    QObject::connect(outlaw_parser,
                     &OutlawParser::gpsReceived,
                     holder,
                     &TelemetryLogHolder::newRocketPosition,
                     Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::batteryUpdated,
                     holder,
                     &TelemetryLogHolder::newBatteryInformation,
                     Qt::QueuedConnection);

    QObject::connect(radio_parser,
                     &RadioPacketParser::motorTempUpdated,
                     holder,
                     &TelemetryLogHolder::newMotorTemp,
                     Qt::QueuedConnection);
    QObject::connect(radio_parser,
                     &RadioPacketParser::radioTempUpdated,
                     holder,
                     &TelemetryLogHolder::newRadioTemp,
                     Qt::QueuedConnection);

    engine.loadFromModule("PayloadGS", "Main");

    return app.exec();
}
