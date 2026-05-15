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
#include "socket_helper.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>    // memset
#include <cassert>    // assert
#include <unistd.h>   // close


// Socket constructor is supposed to create a generic socket which
// can be used for both listening and connecting to remote
Socket::Socket(const Sock_type &sock_type, const Ip_type &ip_type)
    : m_sock_type { sock_type },
      m_ip_type   { ip_type }
{
    const int socktype    { Sock_helper::get_sock_type(sock_type) };
    const int addr_family { Sock_helper::get_addr_family(ip_type) };
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

    struct addrinfo *res { Sock_helper::getaddrinfo_res(m_ip_type, port_no) };
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
    if (p == NULL) {
        // TODO: Throw exception
    }

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
    const Sock_type sock_type { TCP };
    const Ip_type   ip_type   { Sock_helper::get_addr_family(addr.sa_family) };

    Socket remote_socket{ remote_sockfd, sock_type, ip_type };
    // TODO: Maybe add an exception??

    return remote_socket;
}


void Socket::connect_to_remote(std::string node, std::string port_no)
{
    struct addrinfo hints {};
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family   = Sock_helper::get_addr_family(m_ip_type);
    hints.ai_socktype = Sock_helper::get_sock_type(m_sock_type);

    struct addrinfo *res {};
    if (getaddrinfo(node.c_str(), port_no.c_str(), &hints, &res) == -1) {
        // TODO: Throw exception
    }

    struct addrinfo *p;
    for (p = res; p != NULL; p = p->ai_next) {
        if (connect(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            // TODO: Store error message in string to throw later if error occurs
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    if (p == NULL) {
        // TODO: Throw exception with error message being retrieved from above
    }

    // Connected successfully to remote
}
