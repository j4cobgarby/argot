#include "network/server.hpp"

bool cmdarg_exists(std::string arg, std::vector<std::string> args) {
    return std::find(args.begin(), args.end(), arg) != args.end();
}

bool cmdarg_has_value(std::string arg, std::vector<std::string> args) {
    if (!cmdarg_exists(arg, args)) return false;
    if (std::find(args.begin(), args.end(), arg) >= args.end() - 1) return false;
    return true;
}

std::string cmdarg_getvalue(std::string arg, std::vector<std::string> args) { 
    int pos = std::find(args.begin(), args.end(), arg) - args.begin();
    return args.at(pos+1);
}

void message_handler(std::string msg) {
    std::cout << "Handler: " << msg << std::endl;
}

int main(int argc, char* argv[]) {
    int port, max_clients;
    std::string motd;

    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(std::string(argv[i]));
    }

    port = cmdarg_has_value("-p", args) ? std::stoi(cmdarg_getvalue("-p", args)) : DEFAULT_PORT;
    max_clients = cmdarg_has_value("-M", args) ? std::stoi(cmdarg_getvalue("-M", args)) : 10;
    motd = cmdarg_has_value("-m", args) ? cmdarg_getvalue("-m", args) : "Hello, world!";

    std::cout << "Port: " << port << std::endl
        << "Max_clients: " << max_clients << std::endl
        << "MOTD: " << motd << std::endl;

    argot::server_config_t conf = {
        .s_motd = motd,
        .s_port = port,
        .max_clients = max_clients
    };
    argot::server_callbacks_t cbs;
    cbs.s_client_message_cb = &message_handler;

    argot::Server server(conf, cbs, 0);
    server.start();

    std::cin.get();

    std::cout << "Shutting down..." << std::endl;
    server.shutdown();
    std::cout << "Done." << std::endl;
}
