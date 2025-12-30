// #include <QApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QPalette>

constexpr QColor white = QColor(255, 255, 255);
constexpr QColor gray1 = QColor(208, 211, 212);
constexpr QColor gray2 = QColor(162, 170, 173);
constexpr QColor gray3 = QColor(124, 135, 142);
constexpr QColor black = QColor(0, 0, 0);

constexpr QColor rit_orange = QColor(247, 105, 2);
constexpr QColor rit_green = QColor(132, 189, 0);
constexpr QColor rit_red = QColor(218, 41, 28);

// blue 0 156 189
// purple 125 85 199
// yellow 246 190 0

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

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
