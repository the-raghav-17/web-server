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
    constexpr Sock_type   SOCK_TYPE  { Sock_type::TCP };
    constexpr Ip_type     IP_TYPE    { Ip_type::IP_ANY };
    // SEE: If PORT_NO can be of some other type
    constexpr char        PORT_NO[]  { "3490" };

    // TODO: Handle all exceptions the socket throws

    Socket socket{ SOCK_TYPE, IP_TYPE, PORT_NO };
    socket.bind();

    constexpr size_t QUEUE_SIZE { 5 };  // Max. no. of connections that can wait to be connected
    socket.listen( QUEUE_SIZE );
}
