#include "radio_server.hpp"
#include "QDebug"
#include <format>

RadioServer::RadioServer(const QString &path, Radio *radio) : QLocalServer(), path(path), radio(radio) {
    if (QFile::exists(path)) {
        throw PathForServerAlreadyInUse{};
    }
    setListenBacklogSize(1);
    setMaxPendingConnections(1);
    connect(this, &QLocalServer::newConnection, this, &RadioServer::handleNewConnection);

    radio->set_callbacks([this]() { this->tx_done_cb(); },
                         [this](std::span<uint8_t> data, float snr, int16_t rssi, int freq_error) {
                             this->rx_cb(data, snr, rssi, freq_error);
                         });
}
RadioServer::~RadioServer() {
    bool removed = removeServer(path);
    qDebug("Removed Server: %d", (int)removed);
}
bool RadioServer::startListening() {
    qDebug("starting listening");
    bool started = listen(path);
    qDebug("Listenning? %d", started);
    return started;
}
void RadioServer::stopListening() { this->close(); }
void RadioServer::clientReadyRead() {
    if (active_client == nullptr) {
        qWarning("readyRead Called with no active client wth");
        return;
    }
    while (active_client->canReadLine()) {
        QString line = active_client->readLine();
        line.chop(1);
        qDebug("GotLinFromClient: %s", qPrintable(line));
        Parse(std::string_view{line.toStdString()});
    }
}
void RadioServer::sendToClient(std::string str) {
    if (active_client == nullptr) {
        qWarning("No active client: Would have sent %s", str.c_str());
        return;
    }
    qDebug("SendToClient: %s", str.c_str());
    active_client->write(str.c_str());
    active_client->write("\n");
    active_client->flush();
}
void RadioServer::handleNewConnection() {
    qDebug("handling new");
    QLocalSocket *clientSocket = this->nextPendingConnection();

    if (active_client != nullptr) {
        qWarning("handleNewConnection already had an active client");
        clientSocket->abort();
        return;
    }

    qDebug("got new client");
    active_client = clientSocket;
    // stopListening();
    connect(clientSocket, &QLocalSocket::disconnected, this, &RadioServer::handleDisconnect);
    connect(clientSocket, &QLocalSocket::readyRead, this, &RadioServer::clientReadyRead);
}
void RadioServer::handleDisconnect() {
    qWarning("client disconnected");
    active_client->deleteLater();
    active_client = nullptr;
    startListening();
    qDebug("active client: %p", active_client);
}

void RadioServer::Parse(std::string_view str) {
    // log requested line
    parse(str);
}

void RadioServer::dio1_interrupt() { radio->dio1_interrupt(); }

void RadioServer::bad_parse(const std::string &reason) { sendToClient("BAD_PARSE " + reason + "\n"); }
void RadioServer::unknown_command(std::string_view cmd, std::string_view line) {
    sendToClient("UNKNOWN_COMMAND " + std::string{cmd} + "\n");
}

void RadioServer::param_ask() {}
void RadioServer::reset() {}
void RadioServer::state_ask() {}

void RadioServer::tx_seq_mode(enum TxSeqMode mode) {}
void RadioServer::rx_seq_mode(enum RxSeqMode mode) {}

void RadioServer::tx(uint64_t frequency_hz, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len, int32_t power,
                     uint32_t data_len, uint8_t *data) {
    Radio::Error err = radio->setup_tx(frequency_hz, sf, bw, cr, ldr, preamble_len, power);
    if (err != Radio::Error::Ok) {
        sendToClient("ERROR couldn't setup tx. Reason: " + std::to_string(err));
        return;
    };
    err = radio->tx(data_len, data);
    if (err != Radio::Error::Ok) {
        printf("Failed to start tx\n");
        sendToClient("ERROR couldn't tx. Reason: " + std::to_string(err));
    }
                     }

                     void RadioServer::rx(uint64_t freq, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len) {
                         Radio::Error err = radio->setup_rx(freq, sf, bw, cr, ldr, preamble_len);
                         if (err != Radio::Error::Ok) {
                             printf("Failed to setup rx\n");
                         };
                         err = radio->rx();
                         if (err != Radio::Error::Ok) {
                             printf("Failed to start rx\n");
                         } else {
                             sendToClient(QString{"rxing %1 %2 %3 %4 %5"}.arg(freq).arg(SF_Str(sf)).arg(BW_Str(bw)).arg(CR_Str(cr)).arg(ldr == LDR_On ? "LDRON" : "LDROFF").arg(preamble_len).toStdString());
                         }
                     }
                     void RadioServer::sleep() {}
                     void RadioServer::standby() {}

                     void RadioServer::log(std::string_view log_data) {}
                     void RadioServer::ctrl(std::string_view ctrl_data) {}

                     void RadioServer::tx_done_cb() {
                         sendToClient("txed");
                     }
                     void RadioServer::rx_cb(std::span<uint8_t> data, float snr, int16_t rssi, int freq_error) {
                         auto arr = QByteArray::fromRawData((char *)data.data(), data.size());
                         std::string str64 = arr.toBase64().toStdString();
                         auto str = std::format("rxed {} {} {} {}", snr, rssi, freq_error, str64);
                         sendToClient(str);
                     }
