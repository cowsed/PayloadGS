#include <QCoreApplication>
#include <QFile>
#include <QLocalServer>
#include <QLocalSocket>
#include <QtLogging>
#include "QDebug"
#include "QtLogging"
#include "proto_parser.h"
#include <signal.h>
#include <string>
#include <unistd.h>

class PathForServerAlreadyInUse : std::exception
{};

class RadioServer : public QLocalServer, DebuggingDriver
{
public:
    explicit RadioServer(const QString &path)
        : QLocalServer()
        , path(path)
    {
        if (QFile::exists(path)) {
            throw PathForServerAlreadyInUse{};
        }
        setListenBacklogSize(1);
        setMaxPendingConnections(1);
        connect(this, &QLocalServer::newConnection, this, &RadioServer::handleNewConnection);
    }
    ~RadioServer()
    {
        bool removed = removeServer(path);
        qDebug("Removed Server: %d", (int) removed);
    }
    bool startListening()
    {
        qDebug("starting listening");
        bool started = listen(path);
        qDebug("Listenning? %d", started);
        return started;
    }
    void stopListening() { this->close(); }
    void clientReadyRead()
    {
        qDebug("Ready read");
        if (active_client == nullptr) {
            qWarning("readyRead Called with no active client wth");
            return;
        }
        while (active_client->canReadLine()) {
            QString line = active_client->readLine();
            line.chop(1);
            qDebug("Read line %s", qPrintable(line));
            Parse(std::string_view{line.toStdString()});
            active_client->write("dlog stfu dude\n");
        }
    }

private:
    void handleNewConnection()
    {
        qDebug("handling new");
        QLocalSocket *clientSocket = this->nextPendingConnection();

        if (active_client != nullptr) {
            qWarning("handleNewConnection already had an active)_cliert");
            clientSocket->abort();
            return;
        }

        qDebug("got new client");
        active_client = clientSocket;
        stopListening();
        connect(clientSocket, &QLocalSocket::disconnected, this, &RadioServer::handleDisconnect);
        connect(clientSocket, &QLocalSocket::readyRead, this, &RadioServer::clientReadyRead);
    }
    void handleDisconnect()
    {
        qWarning("client disconnected");
        active_client->deleteLater();
        active_client = nullptr;
        startListening();
        qDebug("active client: %p", active_client);
    }
    QString path;
    QByteArray data;
    QLocalSocket *active_client = nullptr;
};

void catchUnixSignals(const std::vector<int> &quitSignals,
                      const std::vector<int> &ignoreSignals = std::vector<int>())
{
    auto handler = [](int sig) -> void {
        qDebug("========\nrreceived signal = %d\n========", sig);
        QCoreApplication::quit();
    };

    // all these signals will be ignored.
    for (int sig : ignoreSignals)
        signal(sig, SIG_IGN);

    // each of these signals calls the handler (quits the QCoreApplication).
    for (int sig : quitSignals)
        signal(sig, handler);
}

int main(int argc, char *argv[])
{
    // qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", QByteArray("0"));

    QCoreApplication a(argc, argv);
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});

    RadioServer *server = new RadioServer("/tmp/radio_server");
    server->setParent(&a);

    bool ret = server->startListening();

    // QLocalSocket *sock = new QLocalSocket{};
    // QObject::connect(server, &QLocalServer::newConnection, [&]() {
    //     qDebug("new connection happened");
    //     sock->write("hello from client\n im here\n");
    // });

    // QObject::connect(sock, &QLocalSocket::errorOccurred, [](QLocalSocket::LocalSocketError err) {
    // qWarning("error %d", (int) err);
    // });

    // sock->connectToServer("/tmp/radio_server");

    return a.exec();
}
