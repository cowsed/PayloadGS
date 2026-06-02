#ifndef RADIOPACKETPARSER_H
#define RADIOPACKETPARSER_H
#include <QLocalSocket>
#include <QQmlEngine>
#include "QObject"
#include "cubesat_comms/packets_p2g.h"
#include "lorasettings.h"
#include "radioclient.h"
#include <qdatetime.h>
#include <qgeocoordinate.h>

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
    Q_PROPERTY(bool InIdlePosition MEMBER InIdlePosition)
    Q_PROPERTY(bool RadioOverTemp MEMBER RadioOverTemp)
    Q_PROPERTY(bool GPSHasFix MEMBER GPSHasFix)

    QML_VALUE_TYPE(payloadFlags)

public:
    static PayloadFlags fromBits(uint16_t bits);
    bool Active;
    bool Autonomous;
    bool LastArmMovedStalled;
    bool LastServoMoveStalled;
    bool ArmMoving;
    bool ServoMoving;
    bool InIdlePosition;
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

    Q_PROPERTY(LoraSettings *loraSettings READ loraSettings NOTIFY loraSettingsChanged FINAL)

    Q_PROPERTY(QDateTime latestTxDateTime READ latestTxDateTime NOTIFY
                   latestTxDateTimeChanged FINAL)

    Q_PROPERTY(QDateTime latestRxDateTime READ latestRxDateTime NOTIFY
                   latestRxDateTimeChanged FINAL)
                   
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
    Q_INVOKABLE static PayloadFlags statusBitsToFlags(uint16_t bits);

    LoraSettings *loraSettings();
    Q_INVOKABLE QDateTime latestTxDateTime();
    Q_INVOKABLE QDateTime latestRxDateTime();

    Q_INVOKABLE void sendCallsign();
    Q_INVOKABLE void setLoraParams(uint32_t freq_hz,
                                   LoraSettings::SpreadingFactor sf,
                                   LoraSettings::Bandwidth bw,
                                   LoraSettings::CodingRate cr);

    explicit RadioPacketParser(QObject *parent = nullptr);

signals:
    void flightStateUpdated(QDateTime time, enum FlightPhase phase, uint16_t bitflags);
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

    void radioTempUpdated(QDateTime time, double tempC);
    void motorTempUpdated(QDateTime time, double tempC);
    void payloadGPSUpdated(QDateTime time, QGeoCoordinate coord);

    void ramUpdated(QDateTime time, uint32_t avail_bytes, uint32_t free_bytes);
    void storageUpdated(QDateTime time, uint64_t avail_bytes, uint64_t free_bytes);
    void batteryUpdated(QDateTime time, double volts, double amps);

    void armAnglesUpdated(
        QDateTime time, uint8_t yaw, uint8_t shoulder, uint8_t elbow, uint8_t wrist);
    void servoAnglesUpdated(
        QDateTime time, uint8_t yaw, uint8_t shoulder, uint8_t elbow, uint8_t wrist);

    void imageDataReceived(QDateTime time, const ImageData &ssdv_packet);

    void loraSettingsChanged();
    void latestTxDateTimeChanged();
    void latestRxDateTimeChanged();

public slots:
    void startedReceiving(QDateTime time,
                          uint32_t freq_hz,
                          RadioClient::SF sf,
                          RadioClient::BW bw,
                          RadioClient::CR cr,
                          RadioClient::LDR ldr);

    void finishedTransmitting(QDateTime time);

    Q_INVOKABLE void packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet);
    Q_INVOKABLE void b64PacketReceived(QDateTime time, int snr, int rssi, const QString &packet);

private:
    void emitCommandResponse(QDateTime time, const CommandResponse *resp);
    void emitTelemetry(QDateTime time, const Telemetry *telem);
    void sendPacket(size_t len, uint8_t *buf);

    RadioClient *payload_client;
    LoraSettings currentRadioSettings;

    QDateTime lastTxDateTime;
    QDateTime lastRxDateTime;

};

#endif // RADIOPACKETPARSER_H
