#ifndef LORASETTINGS_H
#define LORASETTINGS_H

#include <QObject>
#include <QQmlEngine>

class LoraSettings : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(uint32_t frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(SpreadingFactor spreadingFactor READ spreadingFactor WRITE setSpreadingFactor NOTIFY
                   spreadingFactorChanged)
    Q_PROPERTY(Bandwidth bandwidth READ bandwidth WRITE setBandwidth NOTIFY bandwidthChanged)
    Q_PROPERTY(CodingRate codingRate READ codingRate WRITE setCodingRate NOTIFY codingRateChanged)

public:
    enum class SpreadingFactor { SF5, SF6, SF7, SF8, SF9, SF10, SF11, SF12 };
    Q_ENUM(SpreadingFactor)

    enum class Bandwidth { BW8, BW10, BW15, BW20, BW31, BW41, BW62, BW125, BW250, BW500 };
    Q_ENUM(Bandwidth)

    enum class CodingRate { CR4_5, CR4_6, CR4_7, CR4_8 };
    Q_ENUM(CodingRate)

    LoraSettings() {}
    LoraSettings(uint32_t frequency, SpreadingFactor sf, Bandwidth bw, CodingRate cr)
        : m_frequency(frequency)
        , m_sf(sf)
        , m_bw(bw)
        , m_cr(cr)
    {}

    Q_INVOKABLE QString toString() const
    {
        return QString("LoraSettings(sf: %1, bw: %2, cr: %3, f: %4)")
            .arg(spreadingFactorString(m_sf),
                 bandwidthString(m_bw),
                 codingRateString(m_cr),
                 QString::number(m_frequency));
    }

    static Q_INVOKABLE QString spreadingFactorString(SpreadingFactor sf)
    {
        switch (sf) {
        case SpreadingFactor::SF5:
            return "SF5";
        case SpreadingFactor::SF6:
            return "SF6";
        case SpreadingFactor::SF7:
            return "SF7";
        case SpreadingFactor::SF8:
            return "SF8";
        case SpreadingFactor::SF9:
            return "SF9";
        case SpreadingFactor::SF10:
            return "SF10";
        case SpreadingFactor::SF11:
            return "SF11";
        case SpreadingFactor::SF12:
            return "SF12";

        default:
            return "SF??";
        }
    }

    static Q_INVOKABLE QString codingRateString(CodingRate rate)
    {
        switch (rate) {
        case CodingRate::CR4_5:
            return "CR4/5";
        case CodingRate::CR4_6:
            return "CR4/6";
        case CodingRate::CR4_7:
            return "CR4/7";
        case CodingRate::CR4_8:
            return "CR4/8";
        default:
            return "CR???";
        }
    }

    static Q_INVOKABLE QString bandwidthString(Bandwidth bw)
    {
        switch (bw) {
        case Bandwidth::BW8:
            return "BW8";

        case Bandwidth::BW10:
            return "BW10";

        case Bandwidth::BW15:
            return "BW15";

        case Bandwidth::BW20:
            return "BW20";

        case Bandwidth::BW31:
            return "BW31";

        case Bandwidth::BW41:
            return "BW41";

        case Bandwidth::BW62:
            return "BW62";

        case Bandwidth::BW125:
            return "BW125";

        case Bandwidth::BW250:
            return "BW250";

        case Bandwidth::BW500:
            return "BW500";

        default:
            return "BW??";
        }
    }

    uint32_t frequency() const { return m_frequency; }
    SpreadingFactor spreadingFactor() const { return m_sf; }
    Bandwidth bandwidth() const { return m_bw; }
    CodingRate codingRate() const { return m_cr; }

    void setFrequency(uint32_t new_freq)
    {
        if (new_freq != m_frequency) {
            m_frequency = new_freq;
            emit frequencyChanged();
        }
    }

    void setSpreadingFactor(SpreadingFactor new_sf)
    {
        if (new_sf != m_sf) {
            m_sf = new_sf;
            emit spreadingFactorChanged();
        }
    }
    void setBandwidth(Bandwidth new_bw)
    {
        if (new_bw != m_bw) {
            m_bw = new_bw;
            emit bandwidthChanged();
        }
    }

    void setCodingRate(CodingRate new_rate)
    {
        if (new_rate != m_cr) {
            m_cr = new_rate;
            emit frequencyChanged();
        }
    }

signals:
    void frequencyChanged();
    void spreadingFactorChanged();
    void bandwidthChanged();
    void codingRateChanged();

private:
    uint32_t m_frequency{0};
    SpreadingFactor m_sf{SpreadingFactor::SF7};
    Bandwidth m_bw{Bandwidth::BW125};
    CodingRate m_cr{CodingRate::CR4_5};
};

#endif // LORASETTINGS_H
