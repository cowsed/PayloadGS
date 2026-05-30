#pragma once
#include "proto_parser.h"
#include "sx127x.h"
#include <chrono>
#include <gpiod.hpp>
#include <thread>
#include <span>

class Radio {
  public:
    Radio(const char *name, int spidev, gpiod::chip &gpio, int rst_num, int dio0_num);

    enum Error {
        Ok,
        BadSF,
        BadBW,
        BadCR,
        BadFreq,
        BadLDR,
        BadHeader,
        BadPower,
        BadPreamble,
        BadGeneral,
    };
    static const char *error_to_string(Error error);

    using TxDoneCb = std::function<void()>;
    using RxCb = std::function<void(std::span<uint8_t> data, float snr, int16_t rssi, int freq_error)>;
    void set_callbacks(TxDoneCb tx, RxCb rx);

    int spidev_handle();

    void reset();

    void dio1_interrupt();

    Error setup_tx(uint64_t frequency_hz, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len, int32_t power);
    Error tx(uint32_t data_len, uint8_t *data);

    Error setup_rx(uint64_t freq, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len);
    Error rx();

    Error setup_mod_params(uint64_t freq, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len);

    bool waitForDio(std::chrono::nanoseconds waitfor);
    sx127x *device();

  private:
    static void tx_callback(void *ctx);
    static void rx_callback(void *ctx, uint8_t *data, uint16_t data_length);

    static void cad_callback(void *ctx, int cad_detected);

    std::array<uint8_t, 256> outgoing_data;
    int outgoing_len;

    int m_spidev;
    gpiod::chip &m_gpiochip;

    int m_rst_num;
    int m_dio0_num;

    gpiod::line_request m_req;
    sx127x sxDev;

    TxDoneCb tx_cb;
    RxCb rx_cb;
};
