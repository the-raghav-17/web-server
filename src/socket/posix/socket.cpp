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
#include <iostream>


// Construct a generic socket
Socket::Socket(const int sock_type, const int ip_family,
                const int protocol):
      m_sock_type{ sock_type },
      m_ip_family{ ip_family },
      m_protocol{ protocol }
{
    if ((m_sockfd = socket(ip_family, sock_type, protocol)) == -1) {
        // TODO: Throw exception
        std::cout << "Socket creation failed";
        return;
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
        std::cout << "Socket cration failed\n";
        return;
    }

    int sock_type{ Sock_helper::get_sock_type(sockfd) };
    if (sock_type != SOCK_STREAM && sock_type != SOCK_DGRAM) {
        // TODO: Throw exception
        std::cout << "Socket cration failed\n";
        return;
    }

    // TODO: SEE: Do we need to add a helper for getting protocol of other socket???

    // If ip family and socket type are valid, construct object
    m_sock_type = Sock_helper::convert_sock_type(sock_type);
    m_ip_family = Sock_helper::convert_ip_family(ip_family);
    m_sockfd    = sockfd;
}


Socket::~Socket()
{
    // Close the open socket fd
    if (m_sockfd > 0) {
        close(m_sockfd);
    }
}


Socket::Socket(Socket&& other):
    m_sockfd{ other.m_sockfd },
    m_sock_type{ other.m_sock_type },
    m_ip_family{ other.m_ip_family }

{
    other.m_sockfd = -1;
}


Socket& Socket::operator=(Socket&& other)
{
    if (this == &other) {
        return *this;
    }

    this->close_socket();

    m_sockfd = other.m_sockfd;
    m_sock_type = other.m_sock_type;
    m_ip_family = other.m_ip_family;

    other.m_sockfd = -1;

    return *this;
}


// For manually closing the socket.
// Trying to use the socket object after
// closing the socket is error
void Socket::close_socket()
{
    if (m_sockfd > 0) {
        close(m_sockfd);
        m_sockfd = -1;
    }
}


void Socket::bind(const std::string &port_no)
{
    // res is a linked list which consists of all the
    // info required to bind a socket.

    // SEE: How to add exception when getaddrino_list fails
    auto res{ Sock_helper::getaddrinfo_list(port_no, m_ip_family) };

    std::string err_msg {};

    // Traverse the list and bind
    for (auto p = res.get(); p != nullptr; p = p->ai_next) {
        if (::bind(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            err_msg += strerror(errno);
            err_msg += '\n';
            std::cerr << err_msg;
            continue;
        }

        // socket binded successfully
        return;
    }

    // TODO: Throw exception
    std::cerr << "Binding falied\n";
}


void Socket::listen(const std::size_t queue_size) const
{
    if (::listen(m_sockfd, queue_size) == -1) {
        // TODO: Throw listen exception
        std::cerr << "Listening failed: " << strerror(errno) << '\n';
    }
}


std::pair<Socket, Ip_info> Socket::accept() const
{
    struct sockaddr addr{};
    socklen_t       addrlen{ sizeof(addr) };

    int remote_sockfd{};
    if ((remote_sockfd = accept(m_sockfd, &addr, &addrlen)) == -1) {
        // TODO: Throw exception
        std::cout << "Accept failed\n";
    }
    
    // Connecting to remote means that sock_type must be TCP
    const int         ip_family{ addr.sa_family };
    const std::string ip_string{ Sock_helper::get_ip_string(addr) };

    Socket remote_socket{ remote_sockfd };
    // TODO: Maybe add an exception??
    Ip_info ip_info{ ip_family, ip_string };

    return std::make_pair(std::move(remote_socket), ip_info);
}


Ip_info Socket::connect(const std::string& node, const std::string& port_no) const
{
    auto res { Sock_helper::getaddrinfo_list(port_no, m_ip_family, node) };
    std::string err_msg {};

    for (auto p = res.get(); p != nullptr; p = p->ai_next) {
        if (::connect(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            err_msg += strerror(errno);
            err_msg += '\n';
            continue;
        }
        // Connected successfully to remote
        
        int         ip_family{ p->ai_family };
        std::string ip_string{ Sock_helper::get_ip_string(*(p->ai_addr)) };

        return { ip_family, ip_string };
    }
    // TODO: Throw exception with error message being retrieved from above
    std::cout << "Connect failed\n";
}


int Socket::send(const Socket& remote_sock, const std::string& msg)
{
    int remote_sockfd{ remote_sock.get_sockfd() };
    auto buf{ msg.c_str() };
    auto buf_size{ msg.size() };
    int flags{ 0 };

    int size{};

    if ((size = ::send(remote_sockfd, buf, buf_size, flags)) == -1) {
        // TODO: Throw exceptions
        std::cout << "Send failed\n";
    }

    return size;
}


std::string Socket::recv()
{
    std::string buf{};
    buf.resize(1024);   // randomly chosen
    int flags{ 0 };

    if (::recv(m_sockfd, buf.data(), buf.size(), flags) == -1) {
        // TODO: Throw exception
        std::cout << "recv failed\n";
    }

    return buf;
}
