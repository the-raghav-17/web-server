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
#include <cerrno>


// Construct a generic socket
Socket::Socket(const Sock_type sock_type, const Ip_family ip_family)
    : m_sock_type { sock_type },
      m_ip_family   { ip_family }
{
    const auto socktype    { Sock_helper::convert_sock_type(sock_type) };
    const auto addr_family { Sock_helper::convert_ip_family(ip_family) };
    const auto protocol    { 0 };

    if ((m_sockfd = socket(addr_family, socktype, protocol)) == -1) {
        // TODO: Throw exception
    }

    // TODO: Add setsockopt for socket options
    // SEE: Purpose and options in setsockopt
}


// Encapsulate socket fd into an object.
// Supported socket types are Stream socket and Datagram socket
// Address family of socket fd must be either IPv4 or IPv6
Socket::Socket(const int sockfd)
{
    // Firstly get ip family and socket type

    int ip_family{ Sock_helper::get_ip_family(sockfd) };
    if (ip_family != AF_INET && ip_family != AF_INET6) {
        // TODO: Throw exception
    }

    int sock_type{ Sock_helper::get_sock_type(sockfd) };
    if (sock_type != SOCK_STREAM && sock_type != SOCK_DGRAM) {
        // TODO: Throw exception
    }

    // If ip family and socket type are valid, construct object
    m_sock_type = Sock_helper::convert_sock_type(sock_type);
    m_ip_family   = Sock_helper::convert_ip_family(ip_family);
    m_sockfd    = sockfd;
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

    struct addrinfo *res { Sock_helper::getaddrinfo_res(port_no, m_ip_type) };
    if (res == nullptr) {
        // TODO: Throw exception
    }

    // Traverse the list and bind
    struct addrinfo *p {};
    for (p = res; p != NULL; p = p->ai_next) {
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


void Socket::listen_for_conn(const std::size_t &queue_size) const
{
    if (listen(m_sockfd, queue_size) == -1) {
        // TODO: Throw listen exception
    }
}


std::pair<Socket, Ip_info> Socket::accept_remote_conn() const
{
    struct sockaddr addr{};
    socklen_t       addrlen{};

    int remote_sockfd {};
    if ((remote_sockfd = accept(m_sockfd, &addr, &addrlen)) == -1) {
        // TODO: Throw exception
    }
    
    // Connecting to remote means that sock_type must be TCP
    const Ip_family   ip_family { Sock_helper::convert_ip_family(addr.sa_family) };
    const std::string ip_string { Sock_helper::get_ip_string(addr) };

    Socket remote_socket{ remote_sockfd };
    // TODO: Maybe add an exception??
    Ip_info ip_info{ ip_family, ip_string };

    return { remote_socket, ip_info };
}


void Socket::connect_to_remote(const std::string &node, const std::string &port_no)
{
    struct addrinfo *res { Sock_helper::getaddrinfo_res(port_no, m_ip_type, node) };
    struct addrinfo *p;
    for (p = res; p != NULL; p = p->ai_next) {
        if (connect(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            // TODO: Store error message in string to throw later if error occurs
            continue;
        }
        // FIX: Check if we can instantly return a value instead of first breaking out???
        break;
    }
    freeaddrinfo(res);
    if (p == NULL) {
        // TODO: Throw exception with error message being retrieved from above
    }

    // Connected successfully to remote
}
