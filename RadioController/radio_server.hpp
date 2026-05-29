#include "proto_parser.h"
#include "radio.hpp"
#include <QFile>
#include <QLocalServer>
#include <QLocalSocket>
#include <functional>

class PathForServerAlreadyInUse : std::exception {};

class RadioServer : public QLocalServer, ClientToDriverParser {
  public:
    explicit RadioServer(const QString &path, Radio *radio);
    ~RadioServer();



    bool startListening();
    void stopListening();
    void clientReadyRead();
    void sendToClient(std::string str);

    void Parse(std::string_view str);

    void bad_parse(const std::string &reason) override;
    void unknown_command(std::string_view cmd, std::string_view line) override;

    void param_ask() override;
    void reset() override;
    void state_ask() override;

    void tx_seq_mode(enum TxSeqMode mode) override;
    void rx_seq_mode(enum RxSeqMode mode) override;

    void tx(uint64_t frequency_hz, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len, int32_t power,
            uint32_t data_len, uint8_t *data) override;
    void rx(uint64_t freq, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len) override;
    void sleep() override;
    void standby() override;

    void log(std::string_view log_data) override;
    void ctrl(std::string_view ctrl_data) override;

  public slots:
    void dio1_interrupt();

  private:
    void handleNewConnection();
    void handleDisconnect();

    void tx_done_cb();
    void rx_cb(std::span<uint8_t> data, float snr, int16_t rssi, int freq_error);


    Radio *radio;
    QString path;
    QByteArray data;
    QLocalSocket *active_client = nullptr;
};
