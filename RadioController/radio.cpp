#include "proto_parser.h"
#include "sx127x.h"
#include <chrono>
#include <radio.hpp>
#include <string_view>

Radio::Radio(int spidev, gpiod::chip &gpio, int rst_num, int dio0_num, int cs_num)
    : m_spidev(spidev), m_gpiochip(gpio), m_rst_num(rst_num), m_dio0_num(dio0_num), m_spi_cs_num(cs_num),
      m_req(gpio.prepare_request()
                .set_consumer("RadioController")
                .add_line_settings(
                    cs_num, gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT).set_active_low(true))
                .add_line_settings(m_rst_num, gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT))
                .add_line_settings(m_dio0_num, gpiod::line_settings()
                                                   .set_direction(gpiod::line::direction::INPUT)
                                                   .set_edge_detection(gpiod::line::edge::RISING))
                .do_request()) {

    reset();

    int res = sx127x_create((void *)this, &sxDev);
    if (res != SX127X_OK) {
        printf("Error creating with cs %d: %d\n", m_spi_cs_num, res);
    }
}

int Radio::spidev_handle() { return m_spidev; }

void Radio::activate_spi() {
    if (m_spi_cs_num < 0) {
        return;
    }
    // printf("Activating CS: %d\n", m_spi_cs_num);
    m_req.set_value(m_spi_cs_num, gpiod::line::value::ACTIVE);
}
void Radio::deactivate_spi() {
    if (m_spi_cs_num < 0) {
        return;
    }
    // printf("Deactivating CS: %d\n", m_spi_cs_num);
    m_req.set_value(m_spi_cs_num, gpiod::line::value::INACTIVE);
}

void Radio::reset() {
    m_req.set_value(m_rst_num, gpiod::line::value::INACTIVE);

    std::this_thread::sleep_for(std::chrono::milliseconds(3));

    m_req.set_value(m_rst_num, gpiod::line::value::ACTIVE);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

sx127x *Radio::device() { return &sxDev; }

bool Radio::waitForDio(std::chrono::nanoseconds waitfor) {
    bool event = m_req.wait_edge_events(waitfor); // forever
    if (!event) {
        return false; // timeout
    }
    gpiod::edge_event_buffer buf;

    int events = m_req.read_edge_events(buf);
    if (buf.num_events() < 1) {
        return false; // no events
    }
    // auto event = events.get(0);

    return true;
}

/*
    if (spreading_factor >= 11 && bandwidth_hz <= 125000) {
        return 1;
    }
    if (spreading_factor == 10 && bandwidth_hz <= 62500) {
        return 1;
    }
    return 0
*/
bool compute_ldro(sx127x_sf_t sf, sx127x_bw_t bw) {
    bool bw_gt_125000 = (bw == SX127X_BW_250000 || bw == SX127X_BW_500000);
    bool bw_gt_62500 = (bw == SX127X_BW_125000) || bw_gt_125000;
    bool bw_lte_125000 = !bw_gt_125000;
    bool bw_lte_62500 = !bw_gt_62500;

    bool sf_gte_11 = (sf == SX127X_SF_11) || (sf == SX127X_SF_12);
    if (sf_gte_11 && bw_lte_125000) {
        return true;
    }
    if (sf == SX127X_SF_10 && bw_lte_62500) {
        return true;
    }
    return false;
}

void Radio::dio1_interrupt() { sx127x_handle_interrupt(device()); }

sx127x_sf_t sf_to_sf(SF sf) {
    switch (sf) {
    case SF::SF6:
        return SX127X_SF_6;
    case SF::SF7:
        return SX127X_SF_7;
    case SF::SF8:
        return SX127X_SF_8;
    case SF::SF9:
        return SX127X_SF_9;
    case SF::SF10:
        return SX127X_SF_10;
    case SF::SF11:
        return SX127X_SF_11;
    case SF::SF12:
        return SX127X_SF_12;
    default:
        return SX127X_SF_12;
    }
}
sx127x_bw_t bw_to_bw(BW bw) {
    switch (bw) {
    case BW::BW8:
        return SX127X_BW_7800;
    case BW::BW10:
        return SX127X_BW_10400;
    case BW::BW15:
        return SX127X_BW_15600;
    case BW::BW20:
        return SX127X_BW_20800;
    case BW::BW31:
        return SX127X_BW_31250;
    case BW::BW42:
        return SX127X_BW_41700;
    case BW::BW62:
        return SX127X_BW_62500;
    case BW::BW125:
        return SX127X_BW_125000;
    case BW::BW250:
        return SX127X_BW_250000;
    case BW::BW500:
        return SX127X_BW_500000;

    default:
        return SX127X_BW_500000;
    }
}
sx127x_cr_t cr_to_cr(CR cr) {
    switch (cr) {
    case CR::CR4_5:
        return SX127X_CR_4_5;
    case CR::CR4_6:
        return SX127X_CR_4_6;
    case CR::CR4_7:
        return SX127X_CR_4_7;
    case CR::CR4_8:
        return SX127X_CR_4_8;
    default:
        return SX127X_CR_4_8;
    }
}

void Radio::set_callbacks(TxDoneCb tx, RxCb rx) {
    tx_cb = tx;
    rx_cb = rx;
}
void Radio::tx_callback(void *ctx) {
    Radio *self = (Radio *)ctx;
    printf("Finished tx");
    self->rx();
    self->tx_cb();
}
void Radio::rx_callback(void *ctx, uint8_t *data, uint16_t data_length) {
    Radio *self = (Radio *)ctx;

    uint8_t payload[514];
    const char SYMBOLS[] = "0123456789ABCDEF";
    for (size_t i = 0; i < data_length; i++) {
        uint8_t cur = data[i];
        payload[2 * i] = SYMBOLS[cur >> 4];
        payload[2 * i + 1] = SYMBOLS[cur & 0x0F];
    }
    payload[data_length * 2] = '\0';

    int16_t rssi;
    sx127x_rx_get_packet_rssi(&self->sxDev, &rssi);
    float snr;
    sx127x_lora_rx_get_packet_snr(&self->sxDev, &snr);
    int32_t frequency_error;
    sx127x_rx_get_frequency_error(&self->sxDev, &frequency_error);
    fprintf(stdout, "received: %d %s rssi: %d snr: %f freq_error: %d\n", data_length, payload, rssi, snr,
            frequency_error);

    self->rx_cb(std::span<uint8_t>{data, data_length}, snr, rssi, frequency_error);
}

void Radio::cad_callback(void *ctx, int cad_detected) {
    Radio *self = (Radio *)ctx;
    if (cad_detected == 0) {
        fprintf(stdout, "cad not detected\n");
        sx127x_set_opmod(SX127X_MODE_CAD, SX127X_MODULATION_LORA, &self->sxDev);
        return;
    }
    // put into RX mode first to handle interrupt as soon as possible
    sx127x_set_opmod(SX127X_MODE_RX_CONT, SX127X_MODULATION_LORA, &self->sxDev);
    fprintf(stdout, "cad detected\n");
}

Radio::Error Radio::setup_mod_params(uint64_t frequency_hz, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len) {

    sx127x_sf_t sf_ = sf_to_sf(sf);
    sx127x_bw_t bw_ = bw_to_bw(bw);
    sx127x_cr_t cr_ = cr_to_cr(cr);

    if (sx127x_set_opmod(SX127X_MODE_STANDBY, SX127X_MODULATION_LORA, &sxDev) != 0) {
        // failed to return to standby
        return Error::BadGeneral;
    }
    if (sx127x_set_frequency(frequency_hz, &sxDev) != 0) {
        // failed to set freq
        return Error::BadFreq;
    }
    if (sx127x_lora_reset_fifo(&sxDev) != 0) {
        // failed to setup fifo
        return Error::BadGeneral;
    }
    if (sx127x_lora_set_bandwidth(bw_, &sxDev) != 0) {
        // failed to set bw
        return Error::BadBW;
    }
    if (sx127x_lora_set_implicit_header(NULL, &sxDev) != 0) {
        // failed to header
        return Error::BadGeneral;
    }
    if (sx127x_lora_set_spreading_factor(sf_, &sxDev) != 0) {
        // failed to sf
        return Error::BadSF;
    }
    if (sx127x_lora_set_syncword(0x12, &sxDev) != 0) {
        // failed to sync
        return Error::BadGeneral;
    }
    if (sx127x_lora_set_low_datarate_optimization(ldr == LDR_On, &sxDev) != 0) {
        return Error::BadLDR;
    }
    if (sx127x_set_preamble_length(preamble_len, &sxDev) != 0) {
        // failed to preamble
        return Error::BadPreamble;
    }
    sx127x_tx_header_t header = {.enable_crc = true, .coding_rate = cr_};
    if (sx127x_lora_tx_set_explicit_header(&header, &sxDev) != 0) {
        return Error::BadHeader;
    }

    return Error::Ok;
}

Radio::Error Radio::setup_tx(uint64_t frequency_hz, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len,
                             int32_t power) {

    Radio::Error mod_err = setup_mod_params(frequency_hz, sf, bw, cr, ldr, preamble_len);
    if (mod_err != Radio::Error::Ok) {
        return mod_err;
    }

    if (sx127x_tx_set_pa_config(SX127X_PA_PIN_BOOST, power, &sxDev) != 0) {
        return Error::BadPower;
    }

    return Error::Ok;
}

Radio::Error Radio::tx(uint32_t data_len, uint8_t *data) {
    sx127x_tx_set_callback(tx_callback, (void *)this, &sxDev);

    if (sx127x_lora_tx_set_for_transmission(data, data_len, &sxDev) != 0) {
        return Error::BadGeneral;
    }
    if (sx127x_set_opmod(SX127X_MODE_TX, SX127X_MODULATION_LORA, &sxDev) != 0) {
        return Error::BadGeneral;
    }
    return Error::Ok;
}

Radio::Error Radio::setup_rx(uint64_t freq, SF sf, BW bw, CR cr, LDR ldr, uint32_t preamble_len) {
    Radio::Error err = setup_mod_params(freq, sf, bw, cr, ldr, preamble_len);
    if (err != Radio::Error::Ok) {
        return err;
    }
    if (sx127x_lora_set_ppm_offset(4000, &sxDev) != 0) {
        return Radio::Error::BadGeneral;
    }
    // TODO play around with this, depening on what def of HF is for them
    // if (sx127x_rx_set_lna_boost_hf(true, &sxDev) != 0){
    // return Radio::Error::BadGeneral;
    // }
    if (sx127x_rx_set_lna_gain(SX127X_LNA_GAIN_AUTO, &sxDev) != 0) {
        return Radio::Error::BadGeneral;
    }

    return Radio::Error::Ok;
}

Radio::Error Radio::rx() {
    printf("Starting rx\n");
    sx127x_rx_set_callback(rx_callback, (void *)this, &sxDev);
    sx127x_lora_cad_set_callback(cad_callback, (void *)this, &sxDev);
    if (sx127x_set_opmod(SX127X_MODE_RX_CONT, SX127X_MODULATION_LORA, &sxDev) != 0) {
        return Radio::Error::BadGeneral;
    }
    return Radio::Error::Ok;
}

const char *Radio::error_to_string(Error err) {
    switch (err) {
    case Error::Ok:
        return "Ok";
    case Error::BadSF:
        return "BadSF";
    case Error::BadBW:
        return "BadBW";
    case Error::BadCR:
        return "BadCR";
    case Error::BadFreq:
        return "BadFreq";
    case Error::BadLDR:
        return "BadLDR";
    case Error::BadHeader:
        return "BadHeader";
    case Error::BadPower:
        return "BadPower";
    case Error::BadPreamble:
        return "BadPreamble";
    case Error::BadGeneral:
        return "BadGeneral";
    default:
        return "UNKNOWN";
    };
}