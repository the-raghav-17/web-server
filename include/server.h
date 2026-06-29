/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
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
    // TODO: Add server constructor that enables signal handling andd initialized invariants
    Server(const std::string& path):
        m_root_dir{ path }
    {
        if (!std::filesystem::exists(m_root_dir)) {
            throw Server_error{ path + ": No such path exists" };
        }
    }

    void start() const;

private:
    std::filesystem::path m_root_dir{};
    Parser                m_parser{};

    void handle_client(Socket& remote_socket) const;
    void print_request_details(const Http::Request& msg_request) const;

    std::string generate_response(const Http::Request& request) const;
    std::string generate_response_line(const Http::Response& response) const;
};


#endif // SERVER_H_
