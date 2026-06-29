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


class Server
{
public:
    // TODO: Add server constructor that enables signal handling andd initialized invariants
    void start() const;

private:
    Parser m_parser{};

    void handle_client(Socket& remote_socket) const;
    void print_request_details(const Http::Request& msg_request) const;

    std::string generate_response(const Http::Request& request) const;
    std::string generate_response_line(const Http::Response& response) const;
};


#endif // SERVER_H_
