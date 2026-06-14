#ifndef OUTLAWPARSER_H
#define OUTLAWPARSER_H

#include <QDateTime>
#include <QGeoCoordinate>
#include <QObject>
#include <QQmlEngine>
#include "radioclient.h"

class OutlawParser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString statusLine READ statusLine NOTIFY statusLineChanged FINAL)

    QML_ELEMENT
    QML_SINGLETON
    QML_UNCREATABLE("tied to C++ data source")

public:
    explicit OutlawParser(QObject *parent = nullptr);
    Q_INVOKABLE QString statusLine();

public slots:
    void startedReceiving(QDateTime time,
                          uint32_t freq_hz,
                          RadioClient::SF sf,
                          RadioClient::BW bw,
                          RadioClient::CR cr,
                          RadioClient::LDR ldr);

    void finishedTransmitting(QDateTime time);
    void packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet);

signals:
    void gpsReceived(QDateTime time, QGeoCoordinate coord);
    void statusLineChanged();

private:
    void setStatusLine(QString str);

    RadioClient *payload_client;
    QString statusLine_ = "Nothing yet";
};

#endif // OUTLAWPARSER_H
