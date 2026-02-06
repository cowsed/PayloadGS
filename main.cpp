// #include <QApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QFileSystemWatcher>
#include <QPalette>
#include <QtConcurrentRun>

#include <QIcon>
#include <QOpenGLContext>
#include <QQmlContext>
#include <QSurfaceFormat>
#include "datasource.h"
#include <qquickview.h>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    // format.setVersion(1, 4);
    // format.setRenderableType(QSurfaceFormat::);
    // You can also request other options, like multisampling
    // format.setSampleBuffers(true);

    QSurfaceFormat::setDefaultFormat(format); // Set as the default format for all surfaces

    QApplication app(argc, argv);
    QApplication::setDesktopFileName("PayloadGS");

    QIcon icon{":/assets/images/window_icon.png"};
    if (icon.isNull()) {
        qCritical("Couldnt load window icon");
    }

    app.setWindowIcon(icon);
    QQmlApplicationEngine engine;

    QString working_dir = "/home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir";
    size_t flight_id = 0;

    QString flight_dir = QString("%1/%2").arg(working_dir).arg(flight_id);

    // QObject::connect(&watcher,
    //                  &QFileSystemWatcher::fileChanged,
    //                  &parser,
    //                  &TelemetryLogParser::logUpdated,
    //                  Qt::QueuedConnection);
    QQuickView viewer;
    engine.addImportPath(":/");

    DataSource dataSource(&viewer);

    viewer.rootContext()->setContextProperty("dataSource", &dataSource);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("PayloadGS", "Main");

    return app.exec();
}
