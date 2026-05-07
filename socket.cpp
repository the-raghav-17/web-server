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

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>    // memset
#include <cassert>    // assert
#include <unistd.h>   // close


// Local helper functions
// SEE: These functions are local to file, why not make them friend?
static inline int get_sock_type(const Sock_type &sock_type);
static inline int get_addr_family(const Ip_type &ip_type);
static struct addrinfo *getaddrinfo_res(const Ip_type &ip_type, 
                                        const std::string &port_no);


// Socket constructor is supposed to create a generic socket which
// can be used for both listening and connecting to remote
Socket::Socket(const Sock_type &sock_type, const Ip_type &ip_type)
    : m_sock_type { sock_type },
      m_ip_type   { ip_type }
{
    const int socktype    { get_sock_type(sock_type) };
    const int addr_family { get_addr_family(ip_type) };
    const int protocol    { 0 };

    if ((m_sockfd = socket(addr_family, socktype, protocol)) == -1) {
        // TODO: Throw exception
    }

    // TODO: Add setsockopt for socket options
    // SEE: Purpose and options in setsockopt
}


Socket::~Socket()
{
    // Close the open socket fd
    if (m_sockfd > 0) {
        close(m_sockfd);
    }
}


// For manually closing the socket
void Socket::close_socket()
{
    if (m_sockfd > 0) {
        close(m_sockfd);
    }
}


void Socket::bind_to_port(const std::string &port_no)
{
    // res is a linked list which consists of all the
    // info required to bind a socket.

    struct addrinfo *res { getaddrinfo_res(m_ip_type, port_no) };
    if (res == nullptr) {
        // TODO: Throw exception
    }

    // Traverse the list and bind
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        if (bind(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            continue;
        }
        break;
    }

    freeaddrinfo(res);
    // TODO: Throw exception if we couldn't bind to any address

    // socket binded successfully
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
// IPV4 -> AF_INET; IPV6 -> AF_INET6;
static inline int get_addr_family(const Ip_type &ip_type)
{
    assert(ip_type == Ip_type::IPV4 
        || ip_type == Ip_type::IPV6);

    return ip_type == Ip_type::IPV4 ? AF_INET
        : AF_INET6;
}


// Return the res linked list which consits of
// all the information reqd to bind a socket.
//
// TODO: Make this function more generic
static struct addrinfo *getaddrinfo_res(const Ip_type &ip_type,
                                        const std::string &port_no)
{
    struct addrinfo hints {};
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family = get_addr_family(ip_type);
    hints.ai_flags  = AI_PASSIVE;

    // SEE: Why not use smart pointer here to return res list
    struct addrinfo *res {};
    int status { getaddrinfo(NULL, port_no.c_str(), &hints, &res) };
    if (status == -1) {
        // TODO: Throw exception using gai_strerror()
        return nullptr;
    }

    return res;
}
