#ifndef RADIOPACKETPARSER_H
#define RADIOPACKETPARSER_H
#include "QObject"
#include <qdatetime.h>

/**
 * @brief A RadioPacketParser knows how to interpret a packet and adapt it to the GS application
 * Compare to RadioParser which is for parsing the protocol used to talk to the radio server
 */
class RadioPacketParser : public QObject
{
    Q_OBJECT
public:
    explicit RadioPacketParser(QObject *parent = nullptr);

signals:
    void flightHeartbeat(QDateTime time,
                         struct FlightState state,
                         float lat,
                         float lon,
                         uint16_t altitude,
                         uint16_t s_since_boost,
                         int16_t battery_mV,
                         uint8_t radio_temp);

    void tempsUpdated(QDateTime time, double temp1, double temp2);
    void payloadGPSUpdated(QDateTime time, float latitude, float longitude, int32_t altitude);

    void ramUpdated(QDateTime time, uint32_t avail_bytes, uint32_t free_bytes);
    void storageUpdated(QDateTime time, uint64_t avail_bytes, uint64_t free_bytes);
    void batteryUpdated(QDateTime time, double volts, double amps);

    void armAnglesUpdated(QDateTime time, float yaw, float shoulder, float elbow, float wrist);

    void imageDataReceived(uint8_t image_id, uint16_t block_id, const QByteArray &ssdv_packet);
public slots:
    void packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet);

private:
    qint64 last_read = 0;
};

#endif // RADIOPACKETPARSER_H
