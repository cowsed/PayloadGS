#ifndef TELEMETRYLOGPARSER_H
#define TELEMETRYLOGPARSER_H
#include "QObject"
#include <qdatetime.h>

class TelemetryLogParser : public QObject
{
    Q_OBJECT
public:
    explicit TelemetryLogParser(QObject *parent = nullptr);

    void handleLine(const QString &str);

signals:
    void tempsUpdated(QDateTime time, double temp1, double temp2);
    void payloadGPSUpdated(QDateTime time, double latitude, double longitude, int32_t altitude);

    void ramUpdated(QDateTime time, uint32_t avail_bytes, uint32_t free_bytes);
    void storageUpdated(QDateTime time, uint64_t avail_bytes, uint64_t free_bytes);
    void batteryUpdated(QDateTime time, double volts, double amps);

    void armAnglesUpdated(QDateTime time, float yaw, float shoulder, float elbow, float wrist);

public slots:
    void packet_received(QDateTime time, int snr, int rssi, const QByteArray &packet);

private:
    void handleTemps(QDateTime time, const QStringList &list);
    qint64 last_read = 0;
};

#endif // TELEMETRYLOGPARSER_H
