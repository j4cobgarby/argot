#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <string>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <thread>

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#include "common.hpp"

#include "../json/json.hpp"
using json = nlohmann::json;

#define IN_BUF_LENGTH 1024
#define SELECT_TIMEOUT 10
#define DEFAULT_PORT 5555
#define LISTEN_BACKLOG 5

#ifndef NETWORK_SERVER_HPP
#define NETWORK_SERVER_HPP

namespace argot {

    /* This struct stores init info for the server */
    typedef struct {
        std::string s_motd;
        uint16_t s_port;
        int max_clients;
    } server_config_t;

    /* This struct stores the callbacks that the server calls when certain events 
     * happen. */
    typedef struct {
        void(*s_client_message_cb)(std::string nickname, std::string message) = nullptr;
        void(*s_server_up_cb)() = nullptr;
        void(*s_server_down_cb)() = nullptr;
        void(*s_client_joined_cb)(std::string info) = nullptr;
        void(*s_generic_log_cb)(std::string prefix, std::string info) = nullptr;
    } server_callbacks_t;

    typedef struct {
        int socket;
        std::string nickname;
        std::uint8_t perms; // Look at PERM_* macros
    } client_info_t;

    /* Server class
     ****************
     * This class should be deleted and then reinitialised
     * every time the server is restarted, since this is the
     * best and only reliable way to update all of the settings
     * and kick all connected clients.
     */
    class Server {
    protected:
        struct sockaddr_in addr;
        server_config_t conf;
        server_callbacks_t callbacks;
        std::thread server_thread;
        std::vector<client_info_t> clients;
        
        bool running;
        int gui_in_pipe, 
            listener_fd;
        int internal_msgs[2];

        int create_socket(uint16_t port);

        /* Start a thread running the server, which stops when *running is unset. 
         * Non-blocking*/
        void run_server(void);

        /* Handle the value returned by the select() call */
        void handle_select(const int sel_ret, fd_set& read_fds);

        /* Read bytes from socket into string, making sure
         * all bytes are read, even if there are more than IN_BUF_LENGTH*/
        std::string read_from_socket(int sock, bool&);

        /* Does something based on the contents of msg. msg should be
         * received data from the socket sock */
        void handle_client_msg(int sock, std::string msg);

        bool nickname_taken(std::string nick);
        int client_index(int sock);
        int client_index(std::string nick);
    public:
        Server(server_config_t conf, server_callbacks_t callbacks, int gui_in_pipe);

        /* Non-blocking function to start the server. */
        void start(void);

        /* Stops the server, kills all sockets/connections and does all cleanup */
        void shutdown(void);
    };
}

#endif

/*
 * Server s(...);
 *
 * s.start();
 *
 * ...
 *
 * s.shutdown();
 */
