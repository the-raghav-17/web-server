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


struct Ip_info
{
    int         ip_family;
    std::string ip_string;
};

// TODO: Define exceptions for socket


class Socket
{
public:
    // Create socket from scratch
    Socket(const int sock_type, const int ip_family,
            const int protocol);

    // Create socket object out of socket fd
    Socket(const int sockfd);

    ~Socket();

    // Copying not allowed
    Socket(const Socket&)            = delete;
    Socket& operator=(const Socket&) = delete;

    // Move allowed
    Socket(Socket&& other);
    Socket& operator=(Socket&& other);


    // ========== Server methods ========== //
    void bind(const std::string& port_no);
    void listen(const std::size_t queue_size) const;
    std::pair<Socket, Ip_info> accept() const;


    // ========== Client methods ========== //
    Ip_info connect(const std::string& node, 
                    const std::string& port_no) const;

    // ========== Common methods ========== //
    // TODO: Add flags for send()
    int send(const Socket& remote_sock, const std::string& msg);
    std::string recv();

    // TODO: Add sendto() and recvfrom() (UDP methods)

    int  get_sockfd() const { return m_sockfd; }
    void close_socket();  // manually closing the socket

private:
    int m_sockfd{ -1 }; // -1 suggests no socket is created
    int m_sock_type{};  // SOCK_STREAM or SOCK_DGRAM: Connection type socket supports
    int m_ip_family{};  // AF_INET or AF_INET6: Network type socket supports
    int m_protocol{};   // TODO: m_protocol
};


#endif // SOCKET_H_
