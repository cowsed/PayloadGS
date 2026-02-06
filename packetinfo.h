#ifndef PACKETINFO_H
#define PACKETINFO_H

#include <QObject>
#include <QQmlEngine>

struct PacketInfoIdea
{
    QDateTime recv_time;
    uint8_t length;
    int16_t snr;
    int16_t rssi;

    uint8_t type_flag;
};

class PacketInfo : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    // Q_PROPERTY(uint8_t length READ length WRITE setLength NOTIFY lengthChanged)

public:
    enum class TypeFlag {
        FromPayloadLinkTest,
        FromPayloadLinkNegotiate,
        FromPayloadImage,
        FromPayloadTelem,

        FromGSPayloadLinkTest,
        FromGSPayloadLinkNegotiate,
        FromGSPayloadImage,
        FromGSPayloadTelem,

    };
    Q_ENUM(TypeFlag)

    PacketInfo() {}
    PacketInfo(QDateTime recv_time, uint8_t length, int16_t snr, int16_t rssi, uint8_t type_flag)
        : m_recv_time(recv_time)
        , m_length(length)
        , m_snr(snr)
        , m_rssi(rssi)
        , m_type_flag(type_flag)
    {}

    QDateTime recv_time() { return m_recv_time; }
    uint8_t length() { return m_length; }
    int16_t snr() { return m_snr; }
    int16_t rssi() { return m_rssi; }
    // CodingRate codingRate() { return m_cr; }

signals:
    void frequencyChanged();
    void spreadingFactorChanged();
    void bandwidthChanged();
    void codingRateChanged();

private:
    QDateTime m_recv_time;
    uint8_t m_length;
    int16_t m_snr;
    int16_t m_rssi;
    uint8_t m_type_flag;
};

#endif // PACKETINFO_H
