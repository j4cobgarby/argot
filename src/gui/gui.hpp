/*
 * =====================================================================================
 *
 *       Filename:  gui.cpp
 *
 *    Description:  main gui class etc.
 *
 *        Created:  20/06/18 20:57:25
 *       Compiler:  gcc
 *
 *         Author:  Jacob Garby (), thyself@jacobgarby.co.uk
 *
 * =====================================================================================
 */

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

#include <iostream>

#include "../network/server.hpp"

#ifndef GUI_GUI_HPP
#define GUI_GUI_HPP

namespace gui {
    class Gui {
    private:
        // All windows
        Gtk::Window *main_window = nullptr,
                    *servsettings = nullptr;

        // Dialogs
        Gtk::Dialog *addserver = nullptr;
        Gtk::AboutDialog* about = nullptr;

        Gtk::TreeView   *main_servertree = nullptr;

        Gtk::TextView   *main_conversation_textview = nullptr;

        // Toolbar
        Gtk::ToolButton *main_control_panel_btn = nullptr,
                        *main_join_room_btn = nullptr,
                        *main_about_btn = nullptr;

        // Buttons
        Gtk::Button *send_btn = nullptr,
                    *addserver_cancel_btn = nullptr,
                    *addserver_add_btn = nullptr,
                    *servsettings_start_server = nullptr,
                    *servsettings_stop_server = nullptr;

        // Entries
        Gtk::Entry  *msg_entry = nullptr;

        argot::Server* server;
    public:
        Gui(argot::Server* server);
        ~Gui();

        void test_callback();
        void show_window(std::string)

        void run(Glib::RefPtr<Gtk::Application> app);
    };
}

#endif
