// #include <QApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QFileSystemWatcher>
#include <QPalette>
#include <QtConcurrentRun>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setDesktopFileName("PayloadGS");

    QQmlApplicationEngine engine;

    QString working_dir = "/home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir";
    size_t flight_id = 0;

    QString flight_dir = QString("%1/%2").arg(working_dir).arg(flight_id);

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
