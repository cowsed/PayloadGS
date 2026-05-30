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
    const char *socket_path = argv[1];
    const char * spidev_path = argv[2];
    const char *gpiodev_path = argv[3];
    const char *rst_name = argv[4];
    const char *dio1_name = argv[5];


    if (argc < 6) {
        fprintf(stderr, "server /path/to/socket /dev/spidev /dev/gpiochip rst_pin dio1_pin");
        return -1;
    }
    bool parse_ok = false;
    int rst_pin = QString{rst_name}.toInt(&parse_ok);
    if(!parse_ok){
        fprintf(stderr, "Failed to parse %s to pin number for rst\n", rst_name);
        return -1;
    }
    int dio_pin = QString{dio1_name}.toInt(&parse_ok);
    if(!parse_ok){
        fprintf(stderr, "Failed to parse %s to pin number for dio\n", dio1_name);
        return -1;
    }


    int spi_device_fd = open(spidev_path, O_RDWR);
    if (spi_device_fd < 0) {
        perror("unable to open spi device");
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

    gpiod::chip gpiochip{gpiodev_path};
    Radio *radio =  new Radio{socket_path, spi_device_fd, gpiochip, rst_pin, dio_pin};

    QCoreApplication a(argc, argv);
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});

    RadioServer *server = new RadioServer(socket_path, radio);
    server->setParent(&a);

    InterruptWaiter *waiter = new InterruptWaiter(radio);
    QObject::connect(waiter, &InterruptWaiter::interrupt_occurred, server, &RadioServer::dio1_interrupt);
    QObject::connect(waiter, &InterruptWaiter::finished, waiter, &QObject::deleteLater);
    waiter->start();


    bool ret = server->startListening();

    return a.exec();
}
