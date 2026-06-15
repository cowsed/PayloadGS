#include "outlawparser.h"

#define CONFIG_LICENSED_FREQUENCY

#ifdef CONFIG_LICENSED_FREQUENCY
inline constexpr size_t CALLSIGN_CHAR_COUNT = 6;
inline constexpr size_t NODE_ID_START_INDEX = CALLSIGN_CHAR_COUNT;
#else
inline constexpr size_t CALLSIGN_CHAR_COUNT = 0;
inline constexpr size_t NODE_ID_START_INDEX = 0;
#endif
inline constexpr uint8_t NOFIX[] = "NOFIX";

static int32_t nanoToMicro(const int64_t nano)
{
    return static_cast<int32_t>(nano / 1'000);
}

static double microToDeg(const int32_t micro)
{
    return static_cast<double>(micro) / 1'000'000.0;
}

static int32_t metersToFeet(const int16_t meters)
{
    return static_cast<int32_t>(std::lround(static_cast<double>(meters) * 3.28084));
}

#pragma pack(push, 1)
struct GnssInfo
{
    int32_t latitude{0};
    int32_t longitude{0};
    int16_t altitude_m{0};
    uint8_t satellites_cnt{0};
    uint8_t fix_status{0};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NoFixFrame
{
#ifdef CONFIG_LICENSED_FREQUENCY
    char callsign[CALLSIGN_CHAR_COUNT]{0};
#endif
    uint8_t node_id{0};
    const uint8_t nofix[sizeof(NOFIX)]{'N', 'O', 'F', 'I', 'X'};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct LoraFrame
{
#ifdef CONFIG_LICENSED_FREQUENCY
    char callsign[CALLSIGN_CHAR_COUNT]{0};
#endif
    uint8_t version{0x02};
    uint8_t node_id{0};
    GnssInfo gnssInfo{};
};
#pragma pack(pop)

inline constexpr size_t GNSS_INFO_SIZE = sizeof(GnssInfo);
inline constexpr size_t NODE_ID_SIZE = 1;
inline constexpr size_t NOFIX_PACKET_SIZE = NODE_ID_SIZE + sizeof(NOFIX) + CALLSIGN_CHAR_COUNT;
inline constexpr size_t MAX_PAYLOAD_SIZE = NODE_ID_SIZE + GNSS_INFO_SIZE + CALLSIGN_CHAR_COUNT;

/** GNSS fix status */
enum gnss_fix_status {
    /** No GNSS fix acquired */
    GNSS_FIX_STATUS_NO_FIX = 0,
    /** GNSS fix acquired */
    GNSS_FIX_STATUS_GNSS_FIX = 1,
    /** Differential GNSS fix acquired */
    GNSS_FIX_STATUS_DGNSS_FIX = 2,
    /** Estimated fix acquired */
    GNSS_FIX_STATUS_ESTIMATED_FIX = 3,
};

QString parseLoraFrame(const LoraFrame &frame,
                       const size_t size,
                       const int16_t rssi,
                       const int8_t snr)
{
    return "";
    QString dump = "";
    dump += QString("Outlaw Node %1: (%2 bytes | %3 dBm | %4 dB):");
    // .arg((int) frame.node_id)
    // .arg((int) size, (int) rssi, (int) snr);

#ifdef CONFIG_LICENSED_FREQUENCY
    QString callsign = QString::fromUtf8(frame.callsign, CALLSIGN_CHAR_COUNT);
    dump += QString("\tCallsign: %.*s").arg(callsign);
#endif
    dump += QString("\tLatitude: %1").arg(microToDeg(frame.gnssInfo.latitude));
    dump += QString("\tLongitude: %1").arg(microToDeg(frame.gnssInfo.longitude));
    dump += QString("\tAltitude: %1 ft").arg(metersToFeet(frame.gnssInfo.altitude_m));
    dump += QString("\tSatellites count: %1").arg(frame.gnssInfo.satellites_cnt);
    switch (frame.gnssInfo.fix_status) {
    case GNSS_FIX_STATUS_NO_FIX:
        dump += ("\tFix status: NO FIX");
        break;
    case GNSS_FIX_STATUS_GNSS_FIX:
        dump += ("\tFix status: FIX");
        break;
    case GNSS_FIX_STATUS_DGNSS_FIX:
        dump += ("\tFix status: DIFF FIX");
        break;
    case GNSS_FIX_STATUS_ESTIMATED_FIX:
        dump += ("\tFix status: EST FIX");
        break;
    default:
        dump += ("\tFIX status: UNKNOWN");
        break;
    }
    return dump;
}

OutlawParser::OutlawParser(QObject *parent)
    : QObject{parent}
{
    const size_t freq = 425180000;
    // /tmp/radio_serverB

    payload_client = new RadioClient();

    QObject::connect(payload_client,
                     &RadioClient::packetReceived,
                     this,
                     &OutlawParser::packetReceived,
                     Qt::QueuedConnection);

    QObject::connect(payload_client, &RadioClient::connected, this, [&]() {
        qDebug("outlaw radio conn");

        // bool ldro = compute_ldro(defaults.spreadingFactor(), defaults.bandwidth());
        this->payload_client->startReceiving(freq,
                                             RadioClient::SF10,
                                             RadioClient::BW125,
                                             RadioClient::CR4_5,
                                             RadioClient::LDR_Off);
    });

    QObject::connect(payload_client, &RadioClient::disconnected, this, [&]() {
        qWarning("outlaw radio disconn");
    });

    QObject::connect(payload_client,
                     &RadioClient::beganReceiving,
                     this,
                     &OutlawParser::startedReceiving,
                     Qt::QueuedConnection);

    QObject::connect(payload_client,
                     &RadioClient::finishedTransmitting,
                     this,
                     &OutlawParser::finishedTransmitting,
                     Qt::QueuedConnection);

    payload_client->connect("/tmp/radio_serverB");
}

void OutlawParser::startedReceiving(QDateTime time,
                                    uint32_t freq_hz,
                                    RadioClient::SF sf,
                                    RadioClient::BW bw,
                                    RadioClient::CR cr,
                                    RadioClient::LDR ldr)
{}

void OutlawParser::finishedTransmitting(QDateTime time) {}

void OutlawParser::packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet)
{
    uint8_t *data = (uint8_t *) packet.data();
    size_t size = packet.size();

    switch (size) {
    case sizeof(LoraFrame): {
        const auto frame = reinterpret_cast<LoraFrame *>(data);
        QString str = parseLoraFrame(*frame, size, rssi, snr);
        bool hasFix = frame->gnssInfo.fix_status != GNSS_FIX_STATUS_NO_FIX;
        QString callsign = QString::fromUtf8(frame->callsign, 6);
        // str += QString("%1-%2: (%3 bytes | %4 dBm | %5 dB):\n")
        //            .arg(callsign)
        //            .arg((int) frame->node_id)
        //            .arg((int) size, (int) rssi, (int) snr);

        QGeoCoordinate coord{(double) microToDeg(frame->gnssInfo.latitude),
                             (double) microToDeg(frame->gnssInfo.longitude),
                             (double) frame->gnssInfo.altitude_m};
        emit gpsReceived(QDateTime::currentDateTime(), coord);
        setStatusLine(str);
        break;
    }
    case NOFIX_PACKET_SIZE: {
        const auto frame = reinterpret_cast<NoFixFrame *>(data);
        QString str = "";
#ifdef CONFIG_LICENSED_FREQUENCY
        QString callsign = QString::fromUtf8(frame->callsign, 6);
        // str += QString("%1-%2: (%3 bytes | %4 dBm | %5 dB):\n")
        //            .arg(callsign)
        //            .arg((int) frame->node_id)
        //            .arg((int) size, (int) rssi, (int) snr);
#else
        qInfo("Node %d: (%d bytes | %d dBm | %d dB):", frame->node_id, size, rssi, snr);
#endif
        str += ("\tNo fix acquired!\n");
        setStatusLine(str);
        break;
    }
    default:
        QString str = "Unknown packet";
        setStatusLine(str);
        break;
    }
}

void OutlawParser::setStatusLine(QString line)
{
    statusLine_ = line;
}

QString OutlawParser::statusLine()
{
    return statusLine_;
}
