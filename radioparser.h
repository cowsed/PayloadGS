#ifndef RADIOPARSER_H
#define RADIOPARSER_H

#include <QObject>
#include <QThread>
#include <qdatetime.h>

class RadioParser : public QObject
{
    Q_OBJECT
public:
    explicit RadioParser(const QString &fifo_path);

    void handleLine(const QString &line);
    int run_watcher();
signals:
    void radio_packet_received(QDateTime time, int snr, int rssi, const QByteArray &packet);
    void radio_reset();
    void radio_connected_changed(bool is_online);
public slots:

private:
    QString fifo_path;
};

#endif // RADIOPARSER_H
