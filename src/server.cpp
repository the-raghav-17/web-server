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


// Overview of everything the server does, happens here
void Server::start() const
{
    Socket socket{ Sock_type::TCP, Ip_type::IPV4 };
}
