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
    assert(sock_type == SOCK_STREAM || sock_type == SOCK_DGRAM);
    assert(ip_family == AF_INET || ip_family == AF_INET6);

    if ((m_sockfd = socket(ip_family, sock_type, protocol)) == -1) {
        const std::string err_msg{ strerror(errno) };
        throw Socket_error{ "Failed to create socket object: " + err_msg };
    }

    int val{ 1 };
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1) {
        const std::string err_msg{ strerror(errno) };
        throw Socket_error{ "Failed to create socket object: setsockopt: " + err_msg };
    }
}


// Encapsulate socket fd into an object.
// Supported socket types are Stream socket and Datagram socket
// Address family of socket fd must be either IPv4 or IPv6
Socket::Socket(int& sockfd)
{
    // Firstly get ip family and socket type

    try {
        m_ip_family = Sock_helper::get_ip_family(sockfd);
    } catch(const Socket_error& err) {
        throw Socket_error{ "Failed to create socket object: get_ip_family: "
                            + std::string{ err.what() } };
    }

    if (m_ip_family != AF_INET && m_ip_family != AF_INET6) {
        throw Socket_error{ "Failed to create socket object: Invalid IP family" };
    }

    try {
        m_sock_type = Sock_helper::get_sock_type(sockfd);
    } catch(const Socket_error& err) {
        throw Socket_error{ "Failed to create socket object: get_sock_type: "
                            + std::string(err.what()) };
    }

    if (m_sock_type != SOCK_STREAM && m_sock_type != SOCK_DGRAM) {
        throw Socket_error{ "Failed to create socket object: Invalid socket type" };
    }

    // If ip family and socket type are valid, construct object
    m_sockfd = sockfd;
    sockfd   = -1;
    // TODO: m_protocol
}


Socket::~Socket()
{
    // Close the open socket fd
    if (m_sockfd > 0) {
        ::close(m_sockfd);
    }
}


Socket::Socket(Socket&& other):
    m_sockfd{ other.m_sockfd },
    m_sock_type{ other.m_sock_type },
    m_ip_family{ other.m_ip_family }
    // TODO: m_protocol

{
    other.m_sockfd = -1;
}


Socket& Socket::operator=(Socket&& other)
{
    if (this == &other) {
        return *this;
    }

    this->close();

    m_sockfd    = other.m_sockfd;
    m_sock_type = other.m_sock_type;
    m_ip_family = other.m_ip_family;
    // TODO: m_protocol

    other.m_sockfd = -1;

    return *this;
}


// For manually closing the socket.
// Trying to use the socket object after
// closing the socket is error
void Socket::close()
{
    if (m_sockfd > 0) {
        ::close(m_sockfd);
        m_sockfd = -1;
    }
}


void Socket::bind(const std::string &port_no) const
{
    // res is a linked list which consists of all the
    // info required to bind a socket.

    std::string err_msg {};
    Sock_helper::Addrinfo_list res{};
    try {
        res = Sock_helper::getaddrinfo_list(port_no, m_ip_family);
    } catch(const Socket_error& err) {
        err_msg = err.what();
        throw Socket_error{ "Binding failed: " + err_msg };
    }

    // Traverse the list and bind
    for (auto p = res.get(); p != nullptr; p = p->ai_next) {
        if (::bind(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            err_msg += strerror(errno);
            err_msg += '\n';
            continue;
        }

        // socket binded successfully
        return;
    }

    throw Socket_error{ "Binding failed: bind: " + err_msg };
}


void Socket::listen(const std::size_t queue_size) const
{
    if (::listen(m_sockfd, queue_size) == -1) {
        throw Socket_error{ "Listen failed: "
                           + std::string{ strerror(errno) } };
    }
}


std::pair<Socket, Ip_info> Socket::accept() const
{
    struct sockaddr addr{};
    socklen_t       addrlen{ sizeof(addr) };

    int remote_sockfd{};
    if ((remote_sockfd = ::accept(m_sockfd, &addr, &addrlen)) == -1) {
        throw Socket_error{ "Accept failed: "
                           + std::string{ strerror(errno) } };
    }
    
    const int         ip_family{ addr.sa_family };
    const std::string ip_string{ Sock_helper::get_ip_string(addr) };
    Ip_info ip_info{ ip_family, ip_string };

    // FIX: Create another constructor that takes sockfd and ipfamily and socktype
    Socket remote_socket{ remote_sockfd };

    return std::make_pair(std::move(remote_socket), ip_info);
}


Ip_info Socket::connect(const std::string& node, const std::string& port_no) const
{
    std::string err_msg{};

    Sock_helper::Addrinfo_list res{};
    try {
        res = Sock_helper::getaddrinfo_list(port_no, m_ip_family, node);
    } catch(const Socket_error& err) {
        err_msg = err.what();
        throw Socket_error{ "Connect failed: " + err_msg };
    }

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

    throw Socket_error{ "Connect failed: connect: " + err_msg };
}


int Socket::send(const std::string& msg) const
{
    auto buf{ msg.c_str() };
    auto buf_size{ msg.size() };
    int flags{ 0 };

    int size{};

    if ((size = ::send(m_sockfd, buf, buf_size, flags)) == -1) {
        throw Socket_error{ "Send failed: "
                            + std::string{ strerror(errno) } };
    }

    return size;
}


std::string Socket::recv() const
{
    std::string buf{};
    buf.resize(1024);   // FIX: randomly chosen recv buffer
    int flags{ 0 };

    if (::recv(m_sockfd, buf.data(), buf.size(), flags) == -1) {
        throw Socket_error{ "Recv failed: "
                            + std::string{ strerror(errno) } };
    }

    return buf;
}
