#ifndef RADIOPACKETPARSER_H
#define RADIOPACKETPARSER_H
#include <QLocalSocket>
#include <QQmlEngine>
#include "QObject"
#include "cubesat_comms/packets_g2p.h"
#include "cubesat_comms/packets_p2g.h"
#include "frontbackdataholder.h"
#include "imagemetadataholder.h"
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
    Q_PROPERTY(bool RuncamOn MEMBER RuncamOn)
    Q_PROPERTY(bool StmBooted MEMBER StmBooted)

    QML_VALUE_TYPE(payloadFlags)

public:
    PayloadFlags() {}
    static PayloadFlags fromBits(uint16_t bits);
    bool Active = false;
    bool Autonomous = false;
    bool LastArmMovedStalled = false;
    bool LastServoMoveStalled = false;
    bool ArmMoving = false;
    bool ServoMoving = false;
    bool InIdlePosition = false;
    bool MotorsOverTemp = false;
    bool RadioOverTemp = false;
    bool GPSHasFix = false;
    bool RuncamOn = false;
    bool StmBooted = false;
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

    Q_PROPERTY(QString statusLine READ statusLine NOTIFY statusLineChanged FINAL)

    Q_PROPERTY(
        int numLeftBeforeResponse READ getNumLeftBeforeResponse NOTIFY numLeftBeforeResponseChanged)

    Q_PROPERTY(FrontBackDataHolder *radioRSSI READ getRadioRSSI NOTIFY radioRSSIChanged)
    Q_PROPERTY(FrontBackDataHolder *radioSNR READ getRadioSNR NOTIFY radioSNRChanged)

    QML_ELEMENT
    QML_SINGLETON
    QML_UNCREATABLE("tied to C++ data source")
public:
    static constexpr size_t MAX_IN_MEM_RADIO_SIGNAL_ENTRIES = 100;

    enum FlightPhaseQML {
        Starting = FlightPhase_Starting,
        Pad = FlightPhase_Pad,
        Expecting = FlightPhase_ExpectingLaunch,
        Flight = FlightPhase_Flight,
        LandedFlipping = FlightPhase_LandedFlipping,
        LandedUnfolding = FlightPhase_Unfolding,
        LandedAutomaticCamera = FlightPhase_AutoCamera,
        LandedManual = FlightPhase_ManualControl,
        Emergency = FlightPhase_Emergency,
    };
    Q_ENUM(FlightPhaseQML);

    Q_INVOKABLE static QString phaseToShortString(FlightPhaseQML phase);
    Q_INVOKABLE static PayloadFlags statusBitsToFlags(uint16_t bits);

    LoraSettings *loraSettings();
    Q_INVOKABLE QDateTime latestTxDateTime();
    Q_INVOKABLE QDateTime latestRxDateTime();
    Q_INVOKABLE QString statusLine();

    Q_INVOKABLE void askForNewFlight();
    Q_INVOKABLE void askForMetadata(uint8_t image_id);
    Q_INVOKABLE void askForBlocks(uint8_t image_id, const std::vector<uint16_t> &block_ids);
    Q_INVOKABLE void askForFlightHeartbeat();
    Q_INVOKABLE void askForLandedHeartbeat();
    Q_INVOKABLE void askForTelemetryInt(uint8_t typ);
    Q_INVOKABLE void askForTelemetry(TelemetryType typ);

    Q_INVOKABLE void askForRuncamOn(bool on);

    Q_INVOKABLE void askToZeroArm(int8_t syaw, int8_t spitch, int8_t epitch, int8_t wpitch);
    Q_INVOKABLE void askToJogMotor(uint8_t motor_id, int16_t millivolts, uint8_t duration_ticks);

    Q_INVOKABLE void askToMoveServo(uint8_t openness,
                                    uint8_t open_travel_time,
                                    uint8_t open_time,
                                    uint8_t close_travel_time,
                                    uint8_t closeness);

    Q_INVOKABLE void askToGoToPosition(int8_t syaw, int8_t spitch, int8_t epitch, int8_t wpitch);
    Q_INVOKABLE void askToGoToPositionAndComeBack(int8_t syaw,
                                                  int8_t spitch,
                                                  int8_t epitch,
                                                  int8_t wpitch);

    Q_INVOKABLE void sendCallsign();

    Q_INVOKABLE void sendArmTarget();
    Q_INVOKABLE void sendToPhase(FlightPhaseQML phase);

    Q_INVOKABLE void takeStillPicture(PhotoTransformQ transform);
    Q_INVOKABLE void setLocalLoraParams(uint32_t freq_hz,
                                        LoraSettings::SpreadingFactor sf,
                                        LoraSettings::Bandwidth bw,
                                        LoraSettings::CodingRate cr);

    Q_INVOKABLE void negotiateLoraParams(uint32_t freq_hz,
                                         LoraSettings::SpreadingFactor sf,
                                         LoraSettings::Bandwidth bw,
                                         LoraSettings::CodingRate cr,
                                         int8_t remoteDbm);
    Q_INVOKABLE void setTxPower(uint8_t new_power);

    void testLink();
    void linkTestFailed();

    Q_INVOKABLE void redialServer();

    int getNumLeftBeforeResponse();
    FrontBackDataHolder *getRadioRSSI();

    FrontBackDataHolder *getRadioSNR();
    explicit RadioPacketParser(QObject *parent = nullptr);

    Q_INVOKABLE const LoraSettings &defaultLoraSettings() const;
signals:
    void flightStateUpdated(QDateTime time,
                            enum FlightPhase phase,
                            uint16_t bitflags,
                            uint16_t s_since_boost);
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
    void flightElapsedUpdated(QDateTime time, int16_t seconds_elapased);

    void ramUpdated(QDateTime time, uint32_t avail_bytes, uint32_t free_bytes);
    void storageUpdated(QDateTime time, uint64_t avail_bytes, uint64_t free_bytes);
    void batteryUpdated(QDateTime time, double volts, double amps);

    void armAnglesUpdated(QDateTime time, int8_t yaw, int8_t shoulder, int8_t elbow, int8_t wrist);
    void servoAnglesUpdated(QDateTime time, uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3);

    void numImagesIncreased(QDateTime time, uint8_t next_image);
    void imageDataReceived(QDateTime time, const ImageData &ssdv_packet);
    void ImageMetadataReceived(QDateTime time, const ImageMetadata &meta);

    void loraSettingsChanged();

    void radioRSSIChanged();
    void radioSNRChanged();

    void latestTxDateTimeChanged();
    void latestRxDateTimeChanged();
    void numLeftBeforeResponseChanged();

    void statusLineChanged();

    void packetReceivedFromRadio(QDateTime time, int snr, int rssi, const QByteArray &packet);
public slots:
    void startedReceiving(QDateTime time,
                          uint32_t freq_hz,
                          RadioClient::SF sf,
                          RadioClient::BW bw,
                          RadioClient::CR cr,
                          RadioClient::LDR ldr);

    void finishedTransmitting(QDateTime time);

    // call these to fake a packet reception
    Q_INVOKABLE void packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet);
    Q_INVOKABLE void b64PacketReceived(QDateTime time, int snr, int rssi, const QString &packet);

private:
    void emitCommandResponse(QDateTime time, const CommandResponse *resp);
    void emitImageMetadata(QDateTime time, const Telemetry *telem);
    void emitTelemetry(QDateTime time, const Telemetry *telem);
    void sendCommand(CommandAndData *cmd);
    void sendPacket(size_t len, uint8_t *buf);
    void sendPacketOtherParams(size_t len, uint8_t *buf, const LoraSettings &settings);
    void setStatusLine(QString str);

    enum NegotiationStage {
        NotNegotiating,
        WaitingForLinkChangeAck,
        WaitingForLinkTestAck,
    };
    struct NegotiationState
    {
        NegotiationStage stage;
        LoraSettings targetSettings;
        int8_t targetDbm;
    };

    LoraSettings negotiatingTo;
    LoraSettings oldStableSettings;

    bool waitingForNegAck = false;

    RadioClient *payload_client;
    QTimer *linkTestExpiredTimer;
    QTimer *keepAliveTimer;
    bool need_keep_alive = false;

    LoraSettings defaultSettings = {43245000,
                                    LoraSettings::SpreadingFactor::SF12,
                                    LoraSettings::Bandwidth::BW125,
                                    LoraSettings::CodingRate::CR4_5};

    LoraSettings currentRadioSettings;
    uint8_t current_power = 14; // 2 to 18

    QString statusLine_ = "not conn";
    FrontBackDataHolder radio_snr;
    FrontBackDataHolder radio_rssi;

    QDateTime lastTxDateTime;
    QDateTime lastRxDateTime;
    int numLeftBeforeResponse = 0;

    uint8_t last_image_id = 0;
    uint16_t last_s_since_boost = 0;
};

#endif // RADIOPACKETPARSER_H
