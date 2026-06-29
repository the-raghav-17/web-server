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
    // FIX: Rewrite the try-catch block of server::start() to handle connection acceptance differently

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
    const std::string request_msg{ remote_socket.recv() };

    log("Message recieved: ");
    std::cout << request_msg << '\n';

    // TODO: client handling
    Http::Request parsed_request{ m_parser.parse_request(request_msg) };

    print_request_details(parsed_request);

    auto response_msg{ generate_response(parsed_request) };
    log("Response:\n" + response_msg);
    remote_socket.send(response_msg);
}


std::string Server::generate_response(const Http::Request& request) const
{
    Http::Response response{};
    response.version = Http::Version::V1_0;

    if (!request.is_valid) {
        response.code = Http::Response_code::BAD_REQUEST;
        return generate_response_line(response);
    }

    // TODO: Check path of the file requested and see if it's a valid file and construct message accordingly
}


std::string Server::generate_response_line(const Http::Response& response) const
{
    std::string response_line{ "HTTP/" };
    response_line += Http::get_http_version_string(response.version);

    response_line += " ";

    response_line += Http::get_code_string(response.code);
    response_line += " ";
    response_line += Http::get_code_msg(response.code);

    response_line += "\n";
    return response_line;
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
    else if (msg_request.method == Method::POST) {
        log("Method: POST");
    }

    log("Request path: " + msg_request.path);
}
