#pragma once
#include <QObject>
#include <QThread>
#include "radio.hpp"

class InterruptWaiter : public QThread {
    Q_OBJECT

  public:
    InterruptWaiter(Radio *radio);
    virtual ~InterruptWaiter() {}

    void run() override;

  signals:
    void interrupt_occurred();

  private:
    Radio *radio;
};
