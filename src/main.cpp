#include "network/server.hpp"

int main() {
    argot::server_config_t conf = {
        .s_motd = "Hello, world!",
        .s_port = 5555,
        .max_clients = 10
    };
    argot::server_callbacks_t cbs;

    argot::Server server(conf, cbs, 0);
    server.start();

    std::cin.get();

    std::cout << "Shutting down..." << std::endl;
    server.shutdown();
    std::cout << "Done." << std::endl;
}
