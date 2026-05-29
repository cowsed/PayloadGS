#include "QDebug"
#include "QtLogging"
#include "interrupt_waiter.h"
#include "proto_parser.h"
#include "radio.hpp"
#include "sx127x.h"
#include <QCoreApplication>
#include <QFile>
#include <QtLogging>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/gpio.h>
#include <linux/spi/spidev.h>
#include <radio_server.hpp>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

void catchUnixSignals(const std::vector<int> &quitSignals, const std::vector<int> &ignoreSignals = std::vector<int>()) {
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

int main(int argc, char *argv[]) {
    if (argc < 7) {
        fprintf(stderr, "server /path/to/socket /dev/spidev /dev/gpiochip rst_pin dio1_pin cs_pin");
        return -1;
    }
    char *spi_fname = argv[1];

    int spi_device_fd = open("/dev/spidev0.0", O_RDWR);
    if (spi_device_fd < 0) {
        perror("unable to open device");
        return -1;
    }

    int mode = SPI_MODE_0; // CPOL=0, CPHA=0
    ioctl(spi_device_fd, SPI_IOC_WR_MODE, &mode);
    int bits_per_word = 0; // means 8 bits
    ioctl(spi_device_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
    int lsb_setting = 0; // MSB
    ioctl(spi_device_fd, SPI_IOC_WR_LSB_FIRST, &lsb_setting);
    int max_speed = 8000000;
    ioctl(spi_device_fd, SPI_IOC_WR_MAX_SPEED_HZ, &max_speed);

    gpiod::chip gpiochip{"/dev/gpiochip0"};
    // Radio *rA = new Radio{spi_device_fd, gpiochip, 19, 13, 26};
    // Radio *rB = new Radio{spi_device_fd, gpiochip, 20, 12, 7};
    Radio *rA =  new Radio{spi_device_fd, gpiochip, 16, 25, 8};

    // qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", QByteArray("0"));

    QCoreApplication a(argc, argv);
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});

    RadioServer *server = new RadioServer("/tmp/radio_serverA", rA);
    server->setParent(&a);

    InterruptWaiter *waiter = new InterruptWaiter(rA);
    QObject::connect(waiter, &InterruptWaiter::interrupt_occurred, server, &RadioServer::dio1_interrupt);
    QObject::connect(waiter, &InterruptWaiter::finished, waiter, &QObject::deleteLater);
    waiter->start();

    uint8_t data[128] = {1, 2, 3, 4, 5, 6};
    // server->tx(433'000'000, SF::SF12, BW::BW125, CR::CR4_5, LDR::LDR_Off, 8, 20, sizeof(data), data);


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
