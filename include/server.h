/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * server.h
 *
 * Declaration of server class.
 */


#ifndef SERVER_H_
#define SERVER_H_


#include "socket.h"
#include "http.h"
#include "parser.h"

#include <filesystem>


class Server_error : public std::runtime_error
{
public:
    Server_error(const std::string& msg):
        runtime_error{ msg }
    {}
};


class Server
{
public:
    Server(const std::string& path):
        m_root_path{ path }
    {
        // Check whether the path exists or not
        if (!std::filesystem::exists(std::filesystem::path{m_root_path})) {
            throw Server_error{ path + ": No such path exists" };
        }
    }

    void start() const;

private:
    std::string m_root_path{};

    void handle_client(Socket& remote_socket) const;
    void print_request_details(const Http::Request& msg_request) const;
};


#endif // SERVER_H_
