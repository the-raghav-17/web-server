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
static inline int         get_sock_type(const Sock_type &sock_type);
static inline int         get_addr_family(const Ip_type &ip_type);
static inline Ip_type     get_addr_family(const int &sa_family_t);
static std::string        get_ip_string(const sockaddr &addr);
static struct addrinfo    *getaddrinfo_res(const Ip_type &ip_type, 
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


// For manually closing the socket.
// Trying to use the socket object after
// closing the socket fd throws exceptions
void Socket::close_socket()
{
    if (m_sockfd > 0) {
        close(m_sockfd);
        m_sockfd = -1;
    }
}


void Socket::bind_to_port(const std::string &port_no)
{
    // res is a linked list which consists of all the
    // info required to bind a socket.

    // FIX: See if m_sockfd is invalid, does this react appropriatly
    struct addrinfo *res { getaddrinfo_res(m_ip_type, port_no) };
    if (res == nullptr) {
        // TODO: Throw exception
    }

    // Traverse the list and bind
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        if (bind(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            // TODO: Throw exception
        }
        break;
    }

    freeaddrinfo(res);
    // TODO: Throw exception if we couldn't bind to any address

    // socket binded successfully
}


void Socket::listen_for_conn(const std::size_t &queue_size)
{
    if (listen(m_sockfd, queue_size) == -1) {
        // TODO: Throw listen exception
    }
}


Socket Socket::accept_remote_conn()
{
    struct sockaddr addr    {};
    socklen_t       addrlen {};

    int remote_sockfd {};
    if ((remote_sockfd = accept(m_sockfd, &addr, &addrlen)) == -1) {
        // TODO: Throw exception
    }
    
    // Connecting to remote means that sock_type must be TCP
    const Sock_type   sock_type   { TCP };
    const Ip_type     ip_type     { get_addr_family(addr.sa_family) };
    const std::string ip_string   { get_ip_string(addr) };

    Socket remote_socket{ remote_sockfd, sock_type, ip_type, ip_string };
    // TODO: Maybe add an exception??

    return remote_socket;
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


// Overloaded variant of the above; does complete opposite
static inline Ip_type get_addr_family(const int &sa_family_t)
{
    assert(sa_family_t == AF_INET
        || sa_family_t == AF_INET6);

    return sa_family_t == AF_INET ? IPV4
        :  IPV6;
}


static std::string get_ip_string(const sockaddr &addr)
{
    char addr_str[INET6_ADDRSTRLEN];
    sa_family_t sa_family { addr.sa_family };

    // This extracts the address part from sockaddr
    void *src = sa_family == AF_INET 
        ? &((struct sockaddr_in *)&addr)->sin_addr)
        : &((struct sockaddr_in6 *)&addr)->sin6_addr);

    inet_ntop(sa_family, src, addr_str, sizeof(addr_str));
    std::string ip_string { addr_str };
    return ip_string;
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
