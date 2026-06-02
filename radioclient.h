#ifndef RADIOCLIENT_H
#define RADIOCLIENT_H

#include <QLocalSocket>
#include <QObject>

class RadioClient : public QObject
{
    Q_OBJECT
public:
    RadioClient();

    enum SF { SF5, SF6, SF7, SF8, SF9, SF10, SF11, SF12 };
    enum BW { BW8, BW10, BW15, BW20, BW31, BW42, BW62, BW125, BW250, BW500, BW200, BW400, BW800 };
    enum CR {
        CR4_5,
        CR4_6,
        CR4_7,
        CR4_8,
    };

    enum LDR {
        LDR_On,
        LDR_Off,
    };
    enum CRC { CRC_On, CRC_Off };
    enum TxSeqMode {
        TxToSleep,
        TxToStandby,
        TxToRx,
    };
    enum RxSeqMode {
        RxToSleep,
        RxToStandby,
        RxToRx,
    };

public slots:
    void connect(QString server_path);
    void startReceiving(uint32_t freq_hz, SF sf, BW bw, CR cr, LDR ldr);
    void transmit(
        uint32_t freq_hz, SF sf, BW bw, CR cr, LDR ldr, int8_t power, const QByteArray &data);
signals:
    void connected();
    void disconnected();

    void beganReceiving(QDateTime time, uint32_t freq_hz, SF sf, BW bw, CR cr, LDR ldr);
    void beganTransmitting(QDateTime time);
    void finishedTransmitting(QDateTime time);

    void packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet);

private:
    void handleLine(const QString &line);
    QString server_path;
    QLocalSocket *sock;
};

#endif // RADIOCLIENT_H
