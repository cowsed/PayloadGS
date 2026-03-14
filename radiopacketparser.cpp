#include "radiopacketparser.h"
#include <QFile>
#include <QtLogging>
#include "packets_p2g.h"

RadioPacketParser::RadioPacketParser(QObject *parent)
    : QObject{parent}
{}

void RadioPacketParser::packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet)
{
    qDebug("Got SNR: %d - RSSI %d", snr, rssi);
    P2GLinkHeader header;
    auto res = unpack_p2g_link_header((uint8_t *) packet.data(), packet.size(), &header);
}
