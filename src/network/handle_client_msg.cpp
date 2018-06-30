/*
 * =====================================================================================
 *
 *       Filename:  handle_client_msg.cpp
 *
 *    Description:  function to handle a message from a client
 *
 *        Created:  17/06/18 00:41:36
 *       Compiler:  gcc
 *
 *         Author:  Jacob Garby (), thyself@jacobgarby.co.uk
 *
 * =====================================================================================
 */

#include "server.hpp"

namespace argot {
    void Server::handle_client_msg(int sock, std::string msg) {
        json msg_json;
        json out_json;
        std::string out_string;
        int msgtype;

        try {
            msg_json = json::parse(msg);
        } catch (std::exception* ex) {
            std::cout << "Failed to parse JSON: " << msg << std::endl;
            return;
        }

        if (!msg_json.count("message_type")) {
            std::cout << "Json didn't contain the a message_type field" << std::endl;
        }

        msgtype = (int)msg_json.at("message_type");

        switch (msgtype) {
        case MSGTYPE_BLANK:
            break;
        case MSGTYPE_SETUP_REQ: /* Initial setup message */
            std::cout << "Setup" << std::endl;
            out_json = {
                {"message_type", MSGTYPE_SETUP_RESP},
                {"status", RESPSTAT_OK}
            };

            if (!msg_json.count("nickname")) {
                out_json.at("status") = RESPSTAT_FIELD_OMITTED;
            }

            if (nickname_taken(msg_json.at("nickname"))) {
                out_json.at("status") = RESPSTAT_NICKNAME_TAKEN;
            }

            if (out_json.at("status") == RESPSTAT_OK) {
                /* Only if after the checks, the setup is still okay, then
                 * perform the setup */
                clients.at(client_index(sock)).nickname = msg_json.at("nickname");
            }
            out_string = out_json.dump();
            send(sock, out_string.c_str(), out_string.length(), 0);

            break;
        case MSGTYPE_CHATMSG:
            std::cout << "Chatmsg" << std::endl;
            try {
                out_json = {
                    {"message_type", MSGTYPE_CHATMSG},
                    {"sender_nickname", clients.at(client_index(sock)).nickname},
                    {"contents", msg_json.at("contents")}
                };
            } catch (std::exception* ex) {
                std::cout << "Error constructing json: " << ex->what() << std::endl;
                break;
            }
    
            if (callbacks.s_client_message_cb)
                (*callbacks.s_client_message_cb)(
                        clients.at(client_index(sock)).nickname, msg_json.at("contents"));

            break;
        default:
            std::cout << "Unrecognised msg type" << std::endl;
            break;
        }
    }
}
