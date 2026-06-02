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
#include "radiopacketparser.h"
#include "telemetrylogholder.h"

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

    LogProvider *logger = engine.singletonInstance<LogProvider *>("PayloadGS", "LogProvider");
    qt_logger = logger;

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
