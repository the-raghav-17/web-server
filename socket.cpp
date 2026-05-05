/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
 */

/*
 * socket.cpp
 *
 * Implementation of socket class.
 * Specific to POSIX API.
 */


#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cassert>


// Local helper functions
static inline int get_sock_type(const Sock_type &sock_type);
static inline int get_addr_family(const Ip_type &ip_type);


Socket::Socket(const Sock_type &sock_type, const Ip_type &ip_type, 
    const std::string &port_no):
    
    m_sock_type { sock_type },
    m_ip_type   { ip_type },
    m_port_no   { port_no }
{
    // TODO: Add exceptions the socket throws

    struct addrinfo hints {};
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family    = get_addr_family(ip_type);
    hints.ai_socktype  = get_sock_type(sock_type);

    // FIX: setting AI_FLAGS to AI_PASSIVE makes this code less generic
    // as this will also happen if its a client application using socket
    // which we don't want in that case
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res {};
    int status {};

    // FIX: port_no is string type, conver to char *
    if ((status = getaddrinfo(NULL, port_no, &hints, &res)) == -1) {
        // TODO: throw exception
    }
}


// Convert Sock_type to its POSIX equivalent
// TCP -> SOCK_STREAM; UDP -> SOCK_DGRAM
static inline int get_sock_type(const Sock_type &sock_type)
{
    assert(sock_type == Sock_type::TCP 
        || sock_type == Sock_type::UDP);

    return sock_type == Sock_type::TCP ? SOCK_STREAM
            : SOCK_DGRAM;
}


// Convert Ip_type to its POSIX equivalent
// IPV4 -> AF_INET; IPV6 -> AF_INET6; IP_ANY -> AF_UNSPEC
static inline int get_addr_family(const Ip_type &ip_type)
{
    assert(ip_type == Ip_type::IPV4 
        || ip_type == Ip_type::IPV6
        || ip_type == Ip_type::IP_ANY);

    return ip_type == Ip_type::IPV4 ? AF_INET
        : ip_type == Ip_type::IPV6  ? AF_INET6
        : AF_UNSPEC;
}
