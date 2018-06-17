/*
 * =====================================================================================
 *
 *       Filename:  handle_select.cpp
 *
 *    Description:  function to handle select ret val
 *
 *        Created:  17/06/18 00:14:53
 *       Compiler:  gcc
 *
 *         Author:  Jacob Garby (), thyself@jacobgarby.co.uk
 *
 * =====================================================================================
 */

#include "server.hpp"

namespace argot {
    void Server::handle_select(const int sel_ret, fd_set& read_fds) {
        if (FD_ISSET(listener_fd, &read_fds)) { /* New client, since listener has something to say */
            socklen_t addrlen = sizeof(addr);
            client_info_t info;

            int newclient = accept(listener_fd, (struct sockaddr*)&addr, &addrlen);
            if (newclient == -1) {
                std::perror("accept");
                return;
            }
            
            if (clients.size() + 1 >= conf.max_clients) {
                close(newclient);
                return;
            }

            info.socket = newclient;
            info.nickname = "unnamedclient" + std::to_string(clients.size());

            std::cout << "New client! (" << info.socket << ")" << std::endl;
            clients.push_back(info);
        }
        else if (FD_ISSET(gui_in_pipe, &read_fds)) { /* Info from the GUI (probably a button) */
            //...
        }
        else if (FD_ISSET(internal_msgs[0], &read_fds)) { /* Internal msg */
            /* Only one byte is expected to be sent internally */
            char msg_byte;
            int amount_read = read(internal_msgs[0], &msg_byte, 1); /* Read one byte from the pipe */
            if (amount_read < 0) {
                std::perror("read");
            } else {
                switch (msg_byte) {
                case 0: /* Msg telling me to bypass this select timeout and skip instantly, probably
                           so that the server can die peacefully */
                    std::cout << "Skipping select" << std::endl;
                    return;
                }
            }
        }
        else { /* Maybe a client was active */
            int active_client = -1;
            bool client_left = false;
            std::string client_msg;

            /* The data must've been from a client, then */
            for (auto sock = clients.begin(); sock != clients.end(); sock++) {
                if (FD_ISSET(sock->socket, &read_fds)) active_client = sock->socket;
            }

            if (active_client == -1) { /* No client was actually active. This select return was triggered 
                                          by a timeout */
                return;
            }

            client_msg = read_from_socket(active_client, client_left);
            if (!client_left) {
                handle_client_msg(active_client, client_msg);
            }
        }
    }
}
