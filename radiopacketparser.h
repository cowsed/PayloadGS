#ifndef RADIOPACKETPARSER_H
#define RADIOPACKETPARSER_H
#include <QQmlEngine>
#include "QObject"
#include "packets_p2g.h"
#include <qdatetime.h>

class PayloadFlags
{
    Q_GADGET

    Q_PROPERTY(bool Active MEMBER Active)
    Q_PROPERTY(bool Autonomous MEMBER Autonomous)
    Q_PROPERTY(bool LastArmMovedStalled MEMBER LastArmMovedStalled)
    Q_PROPERTY(bool LastServoMoveStalled MEMBER LastServoMoveStalled)
    Q_PROPERTY(bool ArmMoving MEMBER ArmMoving)
    Q_PROPERTY(bool ServoMoving MEMBER ServoMoving)
    Q_PROPERTY(bool MotorsOverTemp MEMBER MotorsOverTemp)
    Q_PROPERTY(bool RadioOverTemp MEMBER RadioOverTemp)
    Q_PROPERTY(bool GPSHasFix MEMBER GPSHasFix)

public:
    static PayloadFlags fromBits(uint16_t bits);
    bool Active;
    bool Autonomous;
    bool LastArmMovedStalled;
    bool LastServoMoveStalled;
    bool ArmMoving;
    bool ServoMoving;
    bool MotorsOverTemp;
    bool RadioOverTemp;
    bool GPSHasFix;
};

/**
 * @brief A RadioPacketParser knows how to interpret a packet and adapt it to the GS application
 * Compare to RadioParser which is for parsing the protocol used to talk to the radio server
 */
class RadioPacketParser : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_UNCREATABLE("tied to C++ data source")
public:
    enum FlightPhaseQML {
        Starting,
        Pad,
        Expecting,
        Flight,
        LandedFlipping,
        LandedAutomatic,
        LandedManual,
    };
    Q_ENUM(FlightPhaseQML);

    Q_INVOKABLE static QString phaseToShortString(FlightPhaseQML phase);

    explicit RadioPacketParser(QObject *parent = nullptr);

signals:
    void flightStateUpdated(QDateTime time, struct FlightState state);
    void flightHeartbeat(QDateTime time,
                         struct FlightState state,
                         float lat,
                         float lon,
                         uint16_t altitude,
                         uint16_t s_since_boost,
                         int16_t battery_mV,
                         uint8_t radio_temp);

    void landedHeartbeat(QDateTime time,
                         struct FlightState state,
                         uint8_t next_image_id,
                         uint8_t next_exec_id,
                         const struct ArmTarget &arm_pos,
                         int16_t battery_mV,
                         uint8_t motor_temp,
                         uint8_t radio_temp);

    void tempsUpdated(QDateTime time, double temp1, double temp2);
    void payloadGPSUpdated(QDateTime time, float latitude, float longitude, int32_t altitude);

    void ramUpdated(QDateTime time, uint32_t avail_bytes, uint32_t free_bytes);
    void storageUpdated(QDateTime time, uint64_t avail_bytes, uint64_t free_bytes);
    void batteryUpdated(QDateTime time, double volts, double amps);

    void armAnglesUpdated(QDateTime time, float yaw, float shoulder, float elbow, float wrist);

    void imageDataReceived(QDateTime time, const ImageData &ssdv_packet);
public slots:
    Q_INVOKABLE void packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet);
    Q_INVOKABLE void b64PacketReceived(QDateTime time, int snr, int rssi, const QString &packet);

private:
    void emitCommandResponse(QDateTime time, const CommandResponse *resp);
    void emitTelemetry(QDateTime time, const Telemetry *telem);

    qint64 last_read = 0;
};

#endif // RADIOPACKETPARSER_H
