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
#include "responder.h"
#include "config.h"

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


Server::Server(int argc, char* argv[])
{
    std::string config_file{};

    // Parse the arguments
    if (argc > 2) {
        // TODO: Error
    }
    else if (argc == 2) {
        // Path to config file is provided
        config_file += argv[1];
    }
    else if (argc == 1) {
        // No config file provided; use default
        config_file += DEFAULT_CONFIG_FILE;
    }

    // Parse the config file
    auto config{ Config::parse_config_file(config_file) };

    m_root_path    = config.root_path;
    m_thread_count = config.thread_count;
    m_queue_size   = config.queue_size;

    // Make sure the HTTP content file exists
    if (!std::filesystem::exists(std::filesystem::path(m_root_path))) {
        throw Server_error{ "Root path '" + m_root_path + "' doesn't exist" };
    }

    // At least one thread should be available
    if (m_thread_count < 1) {
        throw Server_error{ "Thread count should be greater than 0" };
    }

    // We can't queue negative number of connections
    if (m_queue_size < 0) {
        throw Server_error{ "Queue size should be at least 0" };
    }

    // All config is setup; the server is ready to work
}


void
Server::start() const
{
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
void
Server::handle_client(Socket& remote_socket) const
{
    const std::string request_msg{ remote_socket.recv() };

    log("Message recieved:\n" + request_msg);

    Responder responder{ request_msg, m_root_path };
    const auto response_msg{ responder.get_response() };
    remote_socket.send(response_msg);
}


void
Server::print_request_details(const Http::Request& msg_request) const
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
