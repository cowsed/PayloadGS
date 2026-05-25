#include "radioparser.h"
#include <QFile>
#include <QtLogging>
RadioParser::RadioParser(const QString &_fifo_path)
    : fifo_path(_fifo_path)
{}

void RadioParser::handleLine(const QString &line)
{
    if (line.length() == 0) {
        return;
    }
    QStringList parts = line.split(" ");
    if (parts.length() < 1) {
        qWarning("Empty line from radio");
        return;
    }
}

int RadioParser::run_watcher()
{
    QFile fifo{fifo_path};
    while (true) {
        int ret = fifo.open(QIODeviceBase::OpenModeFlag::ReadOnly);
        if (ret != 0) {
            qWarning("Failed to open fifo\n");
            continue;
        }
        emit radio_connected_changed(true);

        while (fifo.isOpen()) {
            // QString line = fifo.readLine();
            // handleLine(line);
        }

        emit radio_connected_changed(false);

        QThread::sleep(100);
    }
}
