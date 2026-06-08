#include "radioclient.h"
#include <QDateTime>

RadioClient::RadioClient()
{
    sock = new QLocalSocket{};
    QObject::connect(sock, &QLocalSocket::connected, this, [this]() {
        qDebug("Connected to radio servr");
        emit connected();
    });

    QObject::connect(sock, &QLocalSocket::errorOccurred, [](QLocalSocket::LocalSocketError err) {
        qWarning("socket error %d", (int) err);
    });

    QObject::connect(sock, &QLocalSocket::disconnected, this, [this]() {
        server_path = "";
        emit disconnected();
    });

    QObject::connect(sock, &QLocalSocket::readyRead, this, [this]() {
        while (sock->canReadLine()) {
            QByteArray linebs = sock->readLine();
            QString line = QString::fromUtf8(linebs);
            handleLine(line);
        };
    });
}

const char *RadioClient::SF_Str(RadioClient::SF sf)
{
    switch (sf) {
    case SF::SF5:
        return "SF5";
    case SF::SF6:
        return "SF6";
    case SF::SF7:
        return "SF7";
    case SF::SF8:
        return "SF8";
    case SF::SF9:
        return "SF9";
    case SF::SF10:
        return "SF10";
    case SF::SF11:
        return "SF11";
    case SF::SF12:
        return "SF12";
    }
    return "SF12";
}
const char *RadioClient::BW_Str(RadioClient::BW bw)
{
    switch (bw) {
    case BW::BW8:
        return "BW8";
    case BW::BW10:
        return "BW10";
    case BW::BW15:
        return "BW15";
    case BW::BW20:
        return "BW20";
    case BW::BW31:
        return "BW31";
    case BW::BW42:
        return "BW42";
    case BW::BW62:
        return "BW62";
    case BW::BW125:
        return "BW125";
    case BW::BW250:
        return "BW250";
    case BW::BW500:
        return "BW500";
    case BW::BW200:
        return "BW200";
    case BW::BW400:
        return "BW400";
    case BW::BW800:
        return "BW800";
    };
    return "BW125";
}
const char *RadioClient::CR_Str(CR cr)
{
    switch (cr) {
    case CR::CR4_5:
        return "CR4/5";
    case CR::CR4_6:
        return "CR4/6";
    case CR::CR4_7:
        return "CR4/7";
    case CR::CR4_8:
        return "CR4/8";
    };
    return "CR4/8";
}

// $sf:   (SF5|SF6|SF7|SF8|SF9|SF10|SF11|SF12)
std::optional<RadioClient::SF> parse_sf(QString view)
{
    using SF = RadioClient::SF;
    if (view == "SF5") {
        return SF::SF5;
    } else if (view == "SF6") {
        return SF::SF6;
    } else if (view == "SF7") {
        return SF::SF7;
    } else if (view == "SF8") {
        return SF::SF8;
    } else if (view == "SF9") {
        return SF::SF9;
    } else if (view == "SF10") {
        return SF::SF10;
    } else if (view == "SF11") {
        return SF::SF11;
    } else if (view == "SF12") {
        return SF::SF12;
    }

    return std::nullopt;
}

// $bw:   (BW8|BW10|BW15|BW20|BW31|BW42|BW62|BW125|BW250|BW500|BW200|BW400|BW800)
std::optional<RadioClient::BW> parse_bw(QString view)
{
    using BW = RadioClient::BW;
    if (view == "BW8") {
        return BW::BW8;
    } else if (view == "BW10") {
        return BW::BW10;
    } else if (view == "BW15") {
        return BW::BW15;
    } else if (view == "BW20") {
        return BW::BW20;
    } else if (view == "BW31") {
        return BW::BW31;
    } else if (view == "BW42") {
        return BW::BW42;
    } else if (view == "BW62") {
        return BW::BW62;
    } else if (view == "BW125") {
        return BW::BW125;
    } else if (view == "BW250") {
        return BW::BW250;
    } else if (view == "BW500") {
        return BW::BW500;
    } else if (view == "BW200") {
        return BW::BW200;
    } else if (view == "BW400") {
        return BW::BW400;
    } else if (view == "BW800") {
        return BW::BW800;
    }

    return std::nullopt;
}
// $cr:   (CR4/5|CR4/6|CR4/7|CR4/8)
std::optional<RadioClient::CR> parse_cr(QString view)
{
    using CR = RadioClient::CR;
    if (view == "CR4/5") {
        return CR::CR4_5;
    } else if (view == "CR4/6") {
        return CR::CR4_6;
    } else if (view == "CR4/7") {
        return CR::CR4_7;
    } else if (view == "CR4/8") {
        return CR::CR4_8;
    }
    return std::nullopt;
}
// $ldr:  LDRON|LDROFF low data rate mode
std::optional<RadioClient::LDR> parse_ldr(QString view)
{
    if (view == "LDRON") {
        return RadioClient::LDR::LDR_On;
    } else if (view == "LDROFF") {
        return RadioClient::LDR::LDR_Off;
    }
    return std::nullopt;
}
std::optional<uint64_t> parse_uint(QString view)
{
    bool ok = false;
    uint64_t val = view.toUInt(&ok);
    if (!ok) {
        return std::nullopt;
    }
    return val;
}
std::optional<int64_t> parse_int(QString view)
{
    bool ok = false;
    int64_t val = view.toUInt(&ok);
    if (!ok) {
        return std::nullopt;
    }
    return val;
}

void RadioClient::connect(QString path)
{
    server_path = path;
    sock->connectToServer(server_path);
}

void RadioClient::handleLine(const QString &line)
{
    auto parts = line.trimmed().split(" ");

    if (parts.size() < 0) {
        qDebug("Ignoring empty line");
        return;
    }
    QString ltype = parts[0];
    if (ltype == "rxing" && parts.size() >= 6) {
        auto freq = parse_uint(parts[1]);
        auto sf = parse_sf(parts[2]);
        auto bw = parse_bw(parts[3]);
        auto cr = parse_cr(parts[4]);
        auto ldr = parse_ldr(parts[5]);

        if (freq && sf && bw && cr && ldr) {
            emit beganReceiving(QDateTime::currentDateTime(), *freq, *sf, *bw, *cr, *ldr);
        }
    } else if (ltype == "txing") {
        emit beganTransmitting(QDateTime::currentDateTime());
    } else if (ltype == "txed") {
        emit finishedTransmitting(QDateTime::currentDateTime());
    } else if (ltype == "rxed") {
        int snr = parts[1].toInt();
        int rssi = parts[2].toInt();

        QByteArray::FromBase64Result res = QByteArray::fromBase64Encoding(parts[4].toUtf8());
        if (res.decodingStatus != QByteArray::Base64DecodingStatus::Ok) {
            qDebug("Bad b64 decode from radio server");
            return;
        }
        emit packetReceived(QDateTime::currentDateTime(), snr, rssi, res.decoded);
    } else {
        qWarning("Unknown or malformed Line from radio server %s: %s",
                 qPrintable(server_path),
                 qPrintable(line));
    }
}

void RadioClient::transmit(
    uint32_t freq_hz, SF sf, BW bw, CR cr, LDR ldr, int8_t power, const QByteArray &data)
{
    const QByteArray b64 = data.toBase64();
    auto str = QString("tx %1 %2 %3 %4 %5 8 %6 %7\n")
                   .arg(freq_hz)
                   .arg(SF_Str(sf), BW_Str(bw), CR_Str(cr))
                   .arg(ldr == LDR::LDR_Off ? "LDROFF" : "LDRON")
                   .arg(power)
                   .arg(b64);
    sock->write(str.toUtf8());
    sock->flush();
}

void RadioClient::startReceiving(uint32_t freq_hz, SF sf, BW bw, CR cr, LDR ldr)
{
    auto str = QString("rx %1 %2 %3 %4 %5 8\n")
                   .arg(freq_hz)
                   .arg(SF_Str(sf), BW_Str(bw), CR_Str(cr))
                   .arg((ldr == LDR::LDR_Off ? "LDROFF" : "LDRON"));
    sock->write(str.toUtf8());
    sock->flush();
}