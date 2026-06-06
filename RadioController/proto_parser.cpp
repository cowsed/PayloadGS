#include "proto_parser.h"
#include <QList>
#include <QString>

void ClientToDriverParser::parse(std::string_view str) {
    size_t first_space_i = str.find(" ");
    std::string_view command =
    (first_space_i == std::string::npos) ? std::string_view{str} : str.substr(0, first_space_i);
    std::string_view args = (first_space_i == std::string::npos) ? std::string_view{""} : str.substr(first_space_i + 1);

    // printf("cmd:  %d  %*.s\n", (int)command.size(), (int)command.size(), command.data());
    // printf("args: %d  %*.s\n", (int)args.size(), (int)args.size(), args.data());

    using pair = std::tuple<const char *, std::function<void(std::string_view view)>>;
    std::array<pair, 11> handlers{
        pair{(const char *)"log", [this](std::string_view v) { log(v); }},
        pair{(const char *)"ctrl", [this](std::string_view v) { ctrl(v); }},
        pair{(const char *)"reset", [this](std::string_view) { reset(); }},
        pair{(const char *)"param_ask", [this](std::string_view) { param_ask(); }},
        pair{(const char *)"state_ask", [this](std::string_view) { state_ask(); }},
        pair{(const char *)"standby", [this](std::string_view) { standby(); }},
        pair{(const char *)"sleep", [this](std::string_view) { sleep(); }},

        pair{(const char *)"tx_seq_mode",
            std::bind(&ClientToDriverParser::parse_tx_seq_mode, this, std::placeholders::_1)},
            pair{(const char *)"rx_seq_mode",
                std::bind(&ClientToDriverParser::parse_rx_seq_mode, this, std::placeholders::_1)},
                pair{(const char *)"tx", std::bind(&ClientToDriverParser::parse_tx, this, std::placeholders::_1)},
                pair{(const char *)"rx", std::bind(&ClientToDriverParser::parse_rx, this, std::placeholders::_1)},
    };

    bool handled = false;
    for (auto &[name, handler] : handlers) {
        if (name == command) {
            handler(args);
            handled = true;
        }
    }
    if (!handled) {
        unknown_command(command, str);
    }
}

// $sf:   (SF5|SF6|SF7|SF8|SF9|SF10|SF11|SF12)
std::optional<SF> ClientToDriverParser::parse_sf(QString view) {
    if (view == "SF5") {
        return SF::SF5;
    } else if (view == "SF6") {
        return SF::SF6;
    } else if (view == "SF7") {
        return SF::SF7;
    } else if (view == "SF8") {
        return SF::SF8;
    } else if (view == "SF9") {
        return SF::SF9;
    } else if (view == "SF10") {
        return SF::SF10;
    } else if (view == "SF11") {
        return SF::SF11;
    } else if (view == "SF12") {
        return SF::SF12;
    }

    return std::nullopt;
}

// $bw:   (BW8|BW10|BW15|BW20|BW31|BW42|BW62|BW125|BW250|BW500|BW200|BW400|BW800)
std::optional<BW> ClientToDriverParser::parse_bw(QString view) {

    if (view == "BW8") {
        return BW::BW8;
    } else if (view == "BW10") {
        return BW::BW10;
    } else if (view == "BW15") {
        return BW::BW15;
    } else if (view == "BW20") {
        return BW::BW20;
    } else if (view == "BW31") {
        return BW::BW31;
    } else if (view == "BW42") {
        return BW::BW42;
    } else if (view == "BW62") {
        return BW::BW62;
    } else if (view == "BW125") {
        return BW::BW125;
    } else if (view == "BW250") {
        return BW::BW250;
    } else if (view == "BW500") {
        return BW::BW500;
    } else if (view == "BW200") {
        return BW::BW200;
    } else if (view == "BW400") {
        return BW::BW400;
    } else if (view == "BW800") {
        return BW::BW800;
    }

    return std::nullopt;
}
// $cr:   (CR4/5|CR4/6|CR4/7|CR4/8)
std::optional<CR> ClientToDriverParser::parse_cr(QString view) {
    if (view == "CR4/5") {
        return CR::CR4_5;
    } else if (view == "CR4/6") {
        return CR::CR4_6;
    } else if (view == "CR4/7") {
        return CR::CR4_7;
    } else if (view == "CR4/8") {
        return CR::CR4_8;
    }
    return std::nullopt;
}
// $ldr:  LDRON|LDROFF low data rate mode
std::optional<LDR> ClientToDriverParser::parse_ldr(QString view) {
    if (view == "LDRON") {
        return LDR::LDR_On;
    } else if (view == "LDROFF") {
        return LDR::LDR_Off;
    }
    return std::nullopt;
}
std::optional<uint64_t> ClientToDriverParser::parse_uint(QString view) {
    bool ok = false;
    uint64_t val = view.toUInt(&ok);
    if (!ok) {
        return std::nullopt;
    }
    return val;
}
std::optional<int64_t> ClientToDriverParser::parse_int(QString view) {
    bool ok = false;
    int64_t val = view.toUInt(&ok);
    if (!ok) {
        return std::nullopt;
    }
    return val;
}
std::optional<QByteArray> ClientToDriverParser::parse_b64_data(QString view) {
    // https://doc.qt.io/qt-6/qbytearray.html#fromBase64Encoding
    if (auto result = QByteArray::fromBase64Encoding(view.toUtf8())) {
        return *result;
    } else {
        return std::nullopt;
    }
}

void ClientToDriverParser::parse_tx(std::string_view view) {
    auto help = "invalid tx command. expected 'tx $freq $sf $bw $cr $ldr $plen $pwr $data'";
    QString args = QString::fromUtf8(view.data(), static_cast<int>(view.size()));
    QList<QString> parts = args.split(" ");
    if (parts.size() < 8) {
        bad_parse(help);
        return;
    }
    std::optional<uint64_t> maybe_freq = parse_uint(parts[0].trimmed());
    std::optional<SF> maybe_sf = parse_sf(parts[1].trimmed());
    std::optional<BW> maybe_bw = parse_bw(parts[2].trimmed());
    std::optional<CR> maybe_cr = parse_cr(parts[3].trimmed());
    std::optional<LDR> maybe_ldr = parse_ldr(parts[4].trimmed());
    std::optional<uint64_t> maybe_plen = parse_uint(parts[5].trimmed());
    std::optional<int64_t> maybe_power = parse_int(parts[6].trimmed());
    std::optional<QByteArray> maybe_data = parse_b64_data(parts[7].trimmed());

    using namespace Qt::StringLiterals;
    if (!maybe_freq) {
        bad_parse((QString("couldnt decode freq given: %s") + parts[0]).toStdString());
        return;
    }
    if (!maybe_sf) {
        bad_parse((QString("couldnt decode SF given: %s") + parts[1]).toStdString());
        return;
    }
    if (!maybe_bw) {
        bad_parse((QString("couldnt decode BW given: %s") + parts[2]).toStdString());
        return;
    }
    if (!maybe_cr) {
        bad_parse((QString("couldnt decode CR given: %s") + parts[3]).toStdString());
        return;
    }
    if (!maybe_ldr) {
        bad_parse((QString("couldnt decode LDR given: %s") + parts[4]).toStdString());
        return;
    }
    if (!maybe_plen) {
        bad_parse((QString("decode preamble length given: %s") + parts[5]).toStdString());
        return;
    }
    if (!maybe_power) {
        bad_parse((QString("couldnt decode power given: %s") + parts[6]).toStdString());
        return;
    }
    if (!maybe_data) {
        bad_parse((QString("couldnt decode data given: %s") + parts[7]).toStdString());
        return;
    }

    tx(*maybe_freq, *maybe_sf, *maybe_bw, *maybe_cr, *maybe_ldr, *maybe_plen, *maybe_power, (*maybe_data).size(),
       (uint8_t *)(*maybe_data).data());
}


void ClientToDriverParser::parse_rx(std::string_view view) {
    auto help = "invalid rx command. expected 'rx $freq $sf $bw $cr $ldr $plen'";
    QString args = QString::fromUtf8(view.data(), static_cast<int>(view.size()));
    QList<QString> parts = args.split(" ");
    if (parts.size() < 6) {
        bad_parse(help);
        return;
    }
    std::optional<uint64_t> maybe_freq = parse_uint(parts[0]);
    std::optional<SF> maybe_sf = parse_sf(parts[1]);
    std::optional<BW> maybe_bw = parse_bw(parts[2]);
    std::optional<CR> maybe_cr = parse_cr(parts[3]);
    std::optional<LDR> maybe_ldr = parse_ldr(parts[4]);
    std::optional<uint64_t> maybe_plen = parse_uint(parts[5]);

    using namespace Qt::StringLiterals;
    if (!maybe_freq) {
        bad_parse((QString("couldnt decode freq given: %s") + parts[0]).toStdString());
        return;
    }
    if (!maybe_sf) {
        bad_parse((QString("couldnt decode SF given: %s") + parts[1]).toStdString());
        return;
    }
    if (!maybe_bw) {
        bad_parse((QString("couldnt decode BW given: %s") + parts[2]).toStdString());
        return;
    }
    if (!maybe_cr) {
        bad_parse((QString("couldnt decode CR given: %s") + parts[3]).toStdString());
        return;
    }
    if (!maybe_ldr) {
        bad_parse((QString("couldnt decode LDR given: %s") + parts[4]).toStdString());
        return;
    }
    if (!maybe_plen) {
        bad_parse((QString("decode preamble length given: %s") + parts[5]).toStdString());
        return;
    }

    rx(*maybe_freq, *maybe_sf, *maybe_bw, *maybe_cr, *maybe_ldr, *maybe_plen);
}
