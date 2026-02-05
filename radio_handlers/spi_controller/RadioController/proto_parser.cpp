#include "proto_parser.h"

void ClientToDriverParser::parse(std::string_view str)
{
    size_t first_space_i = str.find(" ");
    std::string_view command = (first_space_i == std::string::npos) ? std::string_view{str}
                                                                    : str.substr(0, first_space_i);
    std::string_view args = (first_space_i == std::string::npos) ? std::string_view{""}
                                                                 : str.substr(first_space_i + 1);

    // printf("cmd:  %d  %*.s\n", (int)command.size(), (int)command.size(), command.data());
    // printf("args: %d  %*.s\n", (int)args.size(), (int)args.size(), args.data());

    using pair = std::tuple<const char *, std::function<void(std::string_view view)>>;
    std::array<pair, 11> handlers{
        pair{(const char *) "log", [this](std::string_view v) { log(v); }},
        pair{(const char *) "ctrl", [this](std::string_view v) { ctrl(v); }},
        pair{(const char *) "reset", [this](std::string_view) { reset(); }},
        pair{(const char *) "param_ask", [this](std::string_view) { param_ask(); }},
        pair{(const char *) "state_ask", [this](std::string_view) { state_ask(); }},
        pair{(const char *) "standby", [this](std::string_view) { standby(); }},
        pair{(const char *) "sleep", [this](std::string_view) { sleep(); }},

        pair{(const char *) "tx_seq_mode",
             std::bind(&ClientToDriverParser::parse_tx_seq_mode, this, std::placeholders::_1)},
        pair{(const char *) "rx_seq_mode",
             std::bind(&ClientToDriverParser::parse_rx_seq_mode, this, std::placeholders::_1)},
        pair{(const char *) "tx",
             std::bind(&ClientToDriverParser::parse_tx, this, std::placeholders::_1)},
        pair{(const char *) "rx",
             std::bind(&ClientToDriverParser::parse_rx, this, std::placeholders::_1)},
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
};
