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
// SEE: These functions are local to file, why not make them friend?
static inline int get_sock_type(const Sock_type &sock_type);
static inline int get_addr_family(const Ip_type &ip_type);


// Socket constructor is supposed to create a generic socket which
// can be used for both listening and connecting to remote
Socket::Socket(const Sock_type &sock_type, const Ip_type &ip_type):
    m_sock_type { sock_type },
    m_ip_type   { ip_type },
    m_sockfd    { -1 }   // -1 suggests no socket has been created
{
    const int socktype    { get_sock_type(sock_type) };
    const int addr_family { get_addr_family(ip_type) };
    const int protocol    { 0 };

    if ((m_sockfd = socket(addr_family, socktype, protocol)) == -1) {
        // TODO: Throw exception
    }
}


Socket::~Socket()
{
    // Close the open socket fd
    if (m_sockfd != -1) {
        close(m_sockfd);
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
