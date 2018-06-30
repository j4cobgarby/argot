#include "server.hpp"

namespace argot {
    Server::Server(server_config_t conf, server_callbacks_t callbacks, int gui_in_pipe)
        : 
            conf(conf),
            callbacks(callbacks),
            gui_in_pipe(gui_in_pipe)
    {
        std::cout << "init" << std::endl;
        if (pipe(internal_msgs) == -1) {
            std::perror("pipe");
        }
    }

    std::string Server::read_from_socket(int sock, bool& client_left) {
        char in_buf[IN_BUF_LENGTH];
        std::memset(in_buf, 0, IN_BUF_LENGTH);
        std::string ret;
        int amount_read = 0, total_read = 0;

        do {
            amount_read = recv(sock, in_buf, IN_BUF_LENGTH, 0);
            total_read += amount_read;

            if (total_read == 0) {
                close(sock);
                for (auto it = clients.begin(); it != clients.end(); it++) {
                    if (it->socket == sock) {
                        clients.erase(it);
                        std::cout << "Client left" << std::endl;
                        client_left = true;
                        break;
                    }
                }
            }
            else if (amount_read == -2) {
                std::perror("recv");
            }
            else {
                ret.append(in_buf);
            }
        } while (amount_read == IN_BUF_LENGTH);

        return ret;
    }

    int Server::create_socket(uint16_t port) {
        int server_fd;
        int opt = 1;
        
        /* Tell kernel to make a socket for me
         * AF_INET: ipv4
         * SOCK_STREAM: TCP */
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            std::perror("Socket create failed.");
            exit(EXIT_FAILURE);
        }

        /* Socket options
         * SO_REUSEADDR: Reuse the address even if it's bound
         * SO_REUSEPORT: Reuse port even if it's bound
         */
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                sizeof(opt))) {
            std::perror("setsockopt failed");
            exit(EXIT_FAILURE);
        }

        addr.sin_family = AF_INET; /* Ipv4 */
        addr.sin_addr.s_addr = INADDR_ANY; /* Allow connections from anywhere */
        addr.sin_port = htons(port); /* port, in network byte order */

        // now bind it to a port specified by addr
        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::perror("bind failed");
            exit(EXIT_FAILURE);
        }

        return server_fd;
    }

    void Server::run_server(void) {
        fd_set read_fds;
        struct timeval read_timeout;
        listener_fd = create_socket(conf.s_port);
        int greatest_fd, sel_ret;

        if (listen(listener_fd, LISTEN_BACKLOG) < 0) {
            std::perror("listen");
            exit(EXIT_FAILURE);
        }

        if (callbacks.s_server_up_cb) (*callbacks.s_server_up_cb)();

        while (running) {
            read_timeout.tv_sec = SELECT_TIMEOUT;
            read_timeout.tv_usec = 0;

            FD_ZERO(&read_fds);
            FD_SET(listener_fd, &read_fds);
            greatest_fd = listener_fd;

            FD_SET(gui_in_pipe, &read_fds);
            greatest_fd = std::max(gui_in_pipe, greatest_fd);

            FD_SET(internal_msgs[0], &read_fds);
            greatest_fd = std::max(internal_msgs[0], greatest_fd);

            for (auto it = clients.begin(); it != clients.end(); it++) {
                FD_SET(it->socket, &read_fds);
                greatest_fd = std::max(it->socket, greatest_fd);
            }

            sel_ret = select(greatest_fd + 1, &read_fds, NULL, NULL, &read_timeout);
            if (sel_ret == -1) { /* invalid return */
                std::perror("select");
                exit(EXIT_FAILURE);
            }
            else { /* select worked properly */
                handle_select(sel_ret, read_fds);
            }
        }

        /* Cleanup */
        close(listener_fd);
        for (auto it = clients.begin(); it != clients.end(); it++) {
            close(it->socket);
        }
    }

    bool Server::nickname_taken(std::string nick) {
        for (auto it = clients.begin(); it != clients.end(); it++) {
            if (nick == it->nickname) return true;
        }
        return false;
    }

    void Server::start(void) {
        running = true;
        server_thread = std::thread(&Server::run_server, this);
        std::cout << "Thread running" << std::endl;
    }

    int Server::client_index(int sock) {
        for (auto it = clients.begin(); it != clients.end(); it++) {
            if (it->socket == sock) return it - clients.begin();
        }
        return -1;
    }

    int Server::client_index(std::string nick) {
        for (auto it = clients.begin(); it != clients.end(); it++) {
            if (it->nickname == nick) return it - clients.begin();
        }
        return -1;
    }

    void Server::shutdown(void) {
        char dat = 0;

        running = false;
        write(internal_msgs[1], &dat, 1);
        server_thread.join();

        if (callbacks.s_server_down_cb) (*callbacks.s_server_down_cb)();
    }
}
