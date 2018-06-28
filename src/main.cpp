#include <gtkmm/aboutdialog.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/textview.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

#include <glibmm.h>

#include "network/server.hpp"
#include "cmdarg.hpp"
#include "gui/gui.hpp"

void message_handler(std::string nick, std::string msg) {
    std::cout << nick << "-> " << msg << std::endl;
}

int main(int argc, char* argv[]) {
    uint16_t port;
    int max_clients;
    std::string motd;

    auto app = Gtk::Application::create(argc, argv, "uk.co.jacobgarby.gtkim");
    gui::Gui main_gui;

    /* Get command-line args */
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(std::string(argv[i]));
    }

    port = cmdarg::has_value("-p", args) 
        ? std::stoi(cmdarg::get_value("-p", args)) : DEFAULT_PORT;

    max_clients = cmdarg::has_value("-M", args) 
        ? std::stoi(cmdarg::get_value("-M", args)) : 10;

    motd = cmdarg::has_value("-m", args) 
        ? cmdarg::get_value("-m", args) : "Hello, world!";

    /* Setup server config */
    argot::server_config_t conf = {
        .s_motd = motd,
        .s_port = port,
        .max_clients = max_clients
    };

    /* Callbacks */
    argot::server_callbacks_t cbs = {
        .s_client_message_cb = &message_handler,
        .s_server_up_cb = nullptr,
        .s_server_down_cb = nullptr,
        .s_client_joined_cb = nullptr,
        .s_generic_log_cb = nullptr
    };

    /* Create and start server */
    argot::Server server(conf, cbs, 0);
    server.start();

    //main_gui.run(app);

    std::cout << "Shutting down..." << std::endl;
    server.shutdown();
    std::cout << "Done." << std::endl;
}

