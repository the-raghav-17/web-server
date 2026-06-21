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


// Overview of everything the server does, happens here
void Server::start() const
{
    Logger log{ "Server" };

    try {
        Socket socket{ SOCK_STREAM, AF_INET, 0 };
    
        const std::string port_no{ "3490" };
        socket.bind(port_no);
        log("Socket binded successfully to port " + port_no);

        socket.listen(10);
        const std::string addr{ "http://localhost:" + port_no };
        log("Listening on " + addr);

        const auto& [remote_socket, ip_info] = socket.accept();
        log("Successfully connected to " + ip_info.ip_string);
    } catch(const Socket_error& err) {
        log("Error: " + std::string{ err.what() });
    }
}
