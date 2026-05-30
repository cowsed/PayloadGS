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
        printf("===== BYTES FROM SOCK ====\n");
        qDebug("Got bytes from sock");
        while (sock->canReadLine()) {
            qDebug("Got line from sock");
            QByteArray linebs = sock->readLine();
            QString line = QString::fromUtf8(linebs);
            handleLine(line);
        };
    });
}

const char *SF_str(RadioClient::SF sf)
{
    using SF = RadioClient::SF;
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
};
const char *BW_Str(RadioClient::BW bw)
{
    using BW = RadioClient::BW;
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
const char *CR_Str(RadioClient::CR cr)
{
    using CR = RadioClient::CR;
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
enum LDR {
    LDR_On,
    LDR_Off,
};

void RadioClient::connect(QString path)
{
    server_path = path;
    sock->connectToServer(server_path);
}

void RadioClient::handleLine(const QString &line)
{
    auto parts = line.split(" ");

    if (parts.size() < 0) {
        qDebug("Ignoring empty line");
        return;
    }
    QString ltype = parts[0];
    if (ltype == "rxing") {
        emit beganReceiving(QDateTime::currentDateTime());
    } else if (ltype == "txing") {
        emit beganTransmitting(QDateTime::currentDateTime());
    } else if (ltype == "txed") {
        emit finishedTransmitting(QDateTime::currentDateTime());
    } else if (ltype == "rxed") {
        int snr = parts[1].toInt();
        int rssi = parts[2].toInt();

        QByteArray::FromBase64Result res = QByteArray::fromBase64Encoding(parts[4].toUtf8());
        if (res.decodingStatus != QByteArray::Base64DecodingStatus::Ok) {
            qDebug("Bad b64 decode");
            return;
        }
        emit packetReceived(QDateTime::currentDateTime(), snr, rssi, res.decoded);
    } else {
        qWarning("Unknown Line from radio server %s: %s", qPrintable(server_path), qPrintable(line));
    }
}

void RadioClient::startReceiving(uint32_t freq_hz, SF sf, BW bw, CR cr, LDR ldr)
{
    auto str = QString("rx %1 %2 %3 %4 %5 8\n")
                   .arg(freq_hz)
                   .arg(SF_str(sf), BW_Str(bw), CR_Str(cr))
                   .arg((ldr == LDR::LDR_Off ? "LDROFF" : "LDRON"));
    sock->write(str.toUtf8());
    sock->flush();
}