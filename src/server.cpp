/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
 */

/*
 * server.cpp
 *
 * Definitions of server class
 */


#include "server.h"
#include "socket.h"
#include "parser.h"

#include <string>
#include <iostream>


class Logger
{
public:
    Logger(const std::string& type):
        m_type{ type } {}

    void operator()(const std::string& msg) {
        std::cout << "[" << m_type << "]: " << msg << '\n';
    }

private:
    std::string m_type{};
};

Logger log{ "Server" };


// Overview of everything the server does, happens here
void Server::start() const
{

    try {
        Socket socket{ SOCK_STREAM, AF_INET, 0 };
    
        const std::string port_no{ "3490" };
        socket.bind(port_no);
        log("Socket binded successfully to port " + port_no);

        socket.listen(10);
        const std::string addr{ "http://localhost:" + port_no };
        log("Listening on " + addr);

        auto [remote_socket, ip_info] = socket.accept();
        log("Successfully connected to " + ip_info.ip_string);

        handle_client(remote_socket);
    } catch(const Socket_error& err) {
        log("Error: " + std::string{ err.what() });
    }
}


void Server::handle_client(Socket& remote_socket) const
{
    using namespace Http;
    const std::string request_msg{ remote_socket.recv() };

    log("Message recieved: ");
    std::cout << request_msg << '\n';

    // TODO: client handling
    Parser parser{};
    Http::Request msg_request{ parser.parse_request(request_msg) };

    // auto msg_reply{ handle_request(msg_reply) };
    // remote_socket.send(msg_reply);

    if (msg_request.is_valid) {
        log("Is valid request");
    }

    if (msg_request.version == Version::V1_1) {
        log("HTTP 1.1");
    } else if (msg_request.version == Version::V1_0) {
        log("HTTP 1.0");
    }

    if (msg_request.method == Method::GET) {
        log("Method: GET");
    }
    else if (msg_request.method == Method::HEAD) {
        log("Method: HEAD");
    }
    else if (msg_request.method == Method::POST) {
        log("Method: POST");
    }

    log("Request path: " + msg_request.path);
}
