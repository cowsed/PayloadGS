#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

class ClientToDriverParser
{
public:
    void parse(std::string_view str);

protected:
    virtual void bad_parse(const std::string &reason) = 0;
    virtual void unknown_command(std::string_view cmd, std::string_view line) = 0;

    enum SF { SF5, SF6, SF7, SF8, SF9, SF10, SF11, SF12 };
    enum BW { BW8, BW10, BW15, BW20, BW31, BW42, BW62, BW125, BW250, BW500, BW200, BW400, BW800 };
    const char *BW_str(BW bw)
    {
        switch (bw) {
        case BW::BW8:
            return "BW8";
        case BW::BW10:
            return "BW10";
        case BW::BW15:
            return "BW15";
        case BW::BW20:
            return "BW20";
        case BW::BW31:
            return "BW31";
        case BW::BW42:
            return "BW42";
        case BW::BW62:
            return "BW62";
        case BW::BW125:
            return "BW125";
        case BW::BW250:
            return "BW250";
        case BW::BW500:
            return "BW500";
        case BW::BW200:
            return "BW200";
        case BW::BW400:
            return "BW400";
        case BW::BW800:
            return "BW800";
        default:
            return "BW???";
        }
    }
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
    enum Header {
        HeaderOn,
        HeaderExplicit = HeaderOn,
        HeaderImplicit,
        HeaderOff = HeaderImplicit,
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
    // required to implement

    // Format:
    // param_ask
    virtual void param_ask() = 0;

    // Format:
    // reset
    virtual void reset() = 0;

    // Format:
    // sleep
    virtual void sleep() = 0;
    // Format:
    // standby
    virtual void standby() = 0;

    // Format:
    // state_ask
    virtual void state_ask() = 0;

    // Format:
    // tx_seq_mode (tx_to_sleep|tx_to_standby|tx_to_rx*)
    virtual void tx_seq_mode(enum TxSeqMode mode) = 0;

    // Format:
    // tx_seq_mode (rx_to_sleep|rx_to_standby|rx_to_rx*)
    virtual void rx_seq_mode(enum RxSeqMode mode) = 0;

    // Format:
    // tx $freq $sf $bw $cr $ldr $plen $hdr $data
    // $freq: decimal number containing frequency in hertz
    // $sf:   (SF5|SF6|SF7|SF8|SF9|SF10|SF11|SF12)
    // $sw:   (BW8|BW10|BW15|BW20|BW31|BW41|BW62|BW125|BW250|BW500|BW200|BW400|BW800)
    // $cr:   (CR4/5|CR4/6|CR4/7|CR4/8)
    // $ldr:  LDRON|LDROFF low data rate mode
    // $plen: preamble length
    // $hdr:  header. HON or HOFF
    // $data hex encoded string of bytes 0123456789abcdef etc
    virtual void tx(uint64_t frequency_hz,
                    SF sf,
                    BW br,
                    CR cr,
                    LDR ldr,
                    uint32_t preamble_len,
                    Header header,
                    uint32_t data_len,
                    uint8_t *data)
        = 0;

    // Format:
    // rx $freq $sf $bw $cr $ldr $plen $hdr $data
    // $freq: decimal number containing frequency in hertz
    // $sf:   (SF5|SF6|SF7|SF8|SF9|SF10|SF11|SF12)
    // $sw:   (BW8|BW10|BW15|BW20|BW31|BW41|BW62|BW125|BW250|BW500|BW200|BW400|BW800)
    // $cr:   (CR4/5|CR4/6|CR4/7|CR4/8)
    // $ldr:  LDRON|LDROFF low data rate mode
    // $plen: preamble length
    // $hdr:  header. HON or HOFF
    virtual void rx(uint64_t freq, SF sf, BW br, CR cr, LDR ldr, uint32_t preamble_len, Header header)
        = 0;

    // optional to implement
    virtual void log(std::string_view log_data) {}
    virtual void ctrl(std::string_view ctrl_data) {}

private:
    void parse_tx_seq_mode(std::string_view view) { tx_seq_mode(TxSeqMode::TxToRx); }
    void parse_rx_seq_mode(std::string_view view) { rx_seq_mode(RxSeqMode::RxToRx); }
    void parse_tx(std::string_view view)
    {
        auto help = "invalid tx command. expected 'tx $freq $sf $bw $cr $ldr $plen $hdr $data'";

        std::array<uint8_t, 4> arr = {1, 2, 3, 4};
        tx(915100000,
           SF::SF6,
           BW::BW125,
           CR::CR4_5,
           LDR::LDR_Off,
           8,
           HeaderOff,
           arr.size(),
           arr.data());
    }
    void parse_rx(std::string_view view)
    {
        auto help = "invalid tx command. expected 'tx $freq $sf $bw $cr $ldr $plen $hdr $data'";

        rx(915'000'000, SF::SF7, BW::BW125, CR::CR4_5, LDR::LDR_Off, 8, Header::HeaderExplicit);
    }
};

class DriverToClientFormatter
{
    static std::string proto_ask() { return "proto_ask"; }
    static std::string log(const std::string &log_data) { return "log " + log_data; }
};

class ClientParser
{
    void parse(const char *line) {}
};

class DebuggingDriver : ClientToDriverParser
{
public:
    void Parse(std::string_view str) { parse(str); }

    void bad_parse(const std::string &reason) override
    {
        std::printf("Bad Parse: %s\n", reason.c_str());
    }

    void unknown_command(std::string_view cmd, std::string_view line) override
    {
        printf("Unknown CMD: %.*s\n", (int) cmd.length(), cmd.data());
    }

    void param_ask() override { printf("param_ask"); }
    void reset() override { printf("Reset\n"); }
    void state_ask() override { printf("state_ask\n"); }

    void tx_seq_mode(enum TxSeqMode mode) override { printf("TX Seq Mode: \n"); }

    void rx_seq_mode(enum RxSeqMode mode) override { printf("RX Seq Mode: \n"); }

    void tx(uint64_t frequency_hz,
            SF sf,
            BW br,
            CR cr,
            LDR ldr,
            uint32_t preamble_len,
            Header header,
            uint32_t data_len,
            uint8_t *data) override
    {
        printf("TX: freq:%lu  %s\n", frequency_hz, BW_str(br));
    }
    virtual void rx(
        uint64_t freq, SF sf, BW br, CR cr, LDR ldr, uint32_t preamble_len, Header header) override
    {
        printf("RX: %s\n", BW_str(br));
    }
    void sleep() override { printf("Sleep\n"); }
    void standby() override { printf("Standby\n"); }

    void log(std::string_view log_data) override
    {
        printf("Log: %.*s\n", (int) log_data.size(), log_data.data());
    }
    void ctrl(std::string_view ctrl_data) override
    {
        printf("Ctrl: %.*s\n", (int) ctrl_data.size(), ctrl_data.data());
    }
};
