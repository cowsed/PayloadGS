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

int main(int argc, char *argv[])
{
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

    // TelemetryLogHolder *holder
    //     = engine.singletonInstance<TelemetryLogHolder *>("PayloadGS", "TelemetryLogHolder");

    ImageDataHolder *holder = engine.singletonInstance<ImageDataHolder *>("PayloadGS",
                                                                          "ImageDataHolder");
    holder->setFlightDir(flight_dir);
    holder->rescanCount();

    Librarian *lib = engine.singletonInstance<Librarian *>("PayloadGS", "Librarian");
    lib->GatherRequestsFromDisk(holder);
    lib->DumpInfo();

    qDebug("Have %zu requests", lib->NumRequests());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("PayloadGS", "Main");

    return app.exec();
}
