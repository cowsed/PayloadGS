#include "interrupt_waiter.h"
#include <QObject>
#include <QThread>
InterruptWaiter::InterruptWaiter(Radio *radio) : radio(radio) {}

void InterruptWaiter::run() {
    while (!QThread::currentThread()->isInterruptionRequested()) {
        bool got_interrupt = radio->waitForDio(std::chrono::milliseconds(20));
        if (got_interrupt) {
            emit interrupt_occurred();
        }
    }
}
