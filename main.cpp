// #include <QApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QFileSystemWatcher>
#include <QPalette>
#include <QtConcurrentRun>
#include "radioparser.h"
#include "telemetrylogparser.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QString working_dir = "/home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir";
    size_t flight_id = 0;

    QString flight_dir = QString("%1/%2").arg(working_dir).arg(flight_id);
    QFileSystemWatcher watcher{&app};
    QString path = flight_dir + "/telemetry.log";
    printf("adding: %s\n", qPrintable(path));
    bool got = watcher.addPath(path);
    printf("Got: %d\n", (int) got);

    const QStringList paths = watcher.files();
    printf("Wathcing %d paths\n", (int) paths.length());
    for (const auto &ipath : paths) {
        printf("Watching %s\n", qPrintable(ipath));
    }

    TelemetryLogParser parser;

    RadioParser radio{
        "/home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/radio_handlers/responses"};

    QFuture<void> ret = QtConcurrent::run([&radio]() { radio.run_watcher(); });

    QObject::connect(&radio,
                     &RadioParser::radio_packet_received,
                     &parser,
                     &TelemetryLogParser::packet_received,
                     Qt::QueuedConnection);

    // QObject::connect(&watcher,
    //                  &QFileSystemWatcher::fileChanged,
    //                  &parser,
    //                  &TelemetryLogParser::logUpdated,
    //                  Qt::QueuedConnection);

    engine.addImportPath(":/");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("PayloadGS", "Main");

    return app.exec();
}
