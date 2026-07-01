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
    auto version{ Http::Version::V1_0 };
    auto method{ request.method };
    std::string path{ process_path(request.path) };

    if (!request.is_valid) {
        auto code{ Http::Response_code::BAD_REQUEST };
        return generate_response_line(version, code);
    }

    // Generate content header
    auto [code, content_header] = generate_content_header(path);
    if (code != Http::Response_code::OK) {
        return generate_response_line(version, code);
    }

    // Generate content body (only if method is GET)
    std::string content_body{};
    if (method == Http::Method::GET) {
        auto [code, body] = generate_content_body(path);
        if (code != Http::Response_code::OK) {
            return generate_response_line(version, code);
        }
        content_body += body;
    }

    // Get the response line
    auto response_code{ Http::Response_code::OK };
    auto response_line{ generate_response_line(version, response_code) };

    std::string response{
        response_line + "\n" + content_header + "\n" + "\n" + content_body
    };

    return response;
}


std::string Server::generate_response_line(const Http::Version& version,
                                  const Http::Response_code& response_code) const
{
    std::string response_line{ "HTTP/" };
    response_line += Http::get_http_version_string(version);

    response_line += " ";

    response_line += Http::get_code_string(response_code);
    response_line += " ";
    response_line += Http::get_code_msg(response_code);

    return response_line;
}


std::pair<Http::Response_code, std::string>
Server::generate_content_header(const std::string& path) const
{
    std::filesystem::path file_path{ path };

    // If given path doesn't exist, or if it does but is a 
    // directory instead of a file then error out
    if (!std::filesystem::exists(file_path)
      || std::filesystem::is_directory(file_path)) {
        
        return { Http::Response_code::PAGE_NOT_FOUND, "" };
    }

    auto file_size{ std::filesystem::file_size(file_path) };

    std::string content_header{ 
        "Content-Length: " + std::to_string(file_size)
    };

    return { Http::Response_code::OK, content_header };

    // TODO: Content type for content header
}


std::pair<Http::Response_code, std::string>
Server::generate_content_body(const std::string& path) const
{
    // TODO: generate_content_body
}


std::string Server::process_path(const std::string& path) const
{
    std::string processed_path{ path };
    if (processed_path == "/") {
        processed_path += "index.html";
    }

    processed_path = m_root_dir.string() + processed_path;
    return processed_path;
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
