/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
 */

/*
 * socket.h
 *
 * The socket interface. Declares socket class.
 */


#ifndef SOCKET_H_
#define SOCKET_H_


#include <string>


enum class Sock_type
{
    TCP, UDP,
};

enum class Ip_type
{
    IPV4, IPV6,
};

// TODO: Define exceptions for socket


// SEE: How can we make socket class more generic
// by defining an interface and creating different implementation
// based on operating system and/or if its client or server application
// that want to create the socket

// TODO: Complete implementation of socket class
class Socket
{
public:
    Socket(const Sock_type &sock_type, const Ip_type &ip_type);
    ~Socket();

    // TODO: add a close method for manually closing socket
    void bind(const std::string &port_no);
    void listen(size_t queue_size);

private:
    // Necessary socket details
    int         m_sockfd    { -1 };  // -1 suggests no socket is created
    Sock_type   m_sock_type {};
    Ip_type     m_ip_type   {};
};


#endif // SOCKET_H_
