/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * server.cpp
 *
 * Definitions of server class
 */


#include "server.h"
#include "socket.h"
#include "parser.h"
#include "responder.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


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


void Server::start() const
{
    // FIX: Rewrite the try-catch block of server::start() to handle connection acceptance differently (use epoll)

    try {
        Socket socket{ SOCK_STREAM, AF_INET, 0 };
    
        const std::string port_no{ "3490" };
        socket.bind(port_no);
        log("Socket binded successfully to port " + port_no);

        socket.listen(10);
        const std::string addr{ "http://localhost:" + port_no };
        log("Listening on " + addr);

        while (1) {
            auto [remote_socket, ip_info] = socket.accept();
            log("Successfully connected to " + ip_info.ip_string);

            handle_client(remote_socket);
        }
    } catch(const Socket_error& err) {
        log("Error: " + std::string{ err.what() });
    }
}


// Recieves the request message, parses it, generates a response
// and sends it
void Server::handle_client(Socket& remote_socket) const
{
    const std::string request_msg{ remote_socket.recv() };

    log("Message recieved:\n" + request_msg);

    Responder responder{ request_msg, m_root_path };
    const auto response_msg{ responder.generate_response() };
    remote_socket.send(response_msg);
}


void Server::print_request_details(const Http::Request& msg_request) const
{
    using namespace Http;

    if (!msg_request.is_valid) {
        std::cout << "Invalid request from the client\n";
    }

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

    log("Request path: " + msg_request.path);
}
