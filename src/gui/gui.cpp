#include "gui.hpp"

namespace gui {
    Gui::Gui(argot::Server* server) : server(server) {
        auto builder = Gtk::Builder::create();
        builder->add_from_resource("/windows/design.glade");

        builder->get_widget("main_window", main_window);
        builder->get_widget("servsettings", servsettings);
        builder->get_widget("addserver", addserver);
        builder->get_widget("about", about);
        builder->get_widget("main_servertree", main_servertree);
        builder->get_widget("main_conversation_textview", main_conversation_textview);
        builder->get_widget("main_control_panel_btn", main_control_panel_btn);
        builder->get_widget("main_join_room_btn", main_join_room_btn);
        builder->get_widget("main_about_btn", main_about_btn);
        builder->get_widget("send_btn", send_btn);
        builder->get_widget("addserver_cancel_btn", addserver_cancel_btn);
        builder->get_widget("addserver_add_btn", addserver_add_btn);
        builder->get_widget("servsettings_start_server", servsettings_start_server);
        builder->get_widget("servsettings_stop_server", servsettings_stop_server);
        builder->get_widget("msg_entry", msg_entry);

        servsettings_start_server->signal_pressed().connect([this]{this->server->start();});
        servsettings_stop_server->signal_pressed().connect([this]{
            this->server->shutdown();
        });
        main_control_panel_btn->signal_clicked().connect([this]{this->servsettings->show();});
    }

    Gui::~Gui() {
        if (!server->is_running()) server->shutdown();
    }

    void Gui::run(Glib::RefPtr<Gtk::Application> app) {
        main_window->show();
        app->run(*main_window);
    }
}
