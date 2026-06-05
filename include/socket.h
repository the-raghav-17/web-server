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

enum class Ip_family
{
    IPV4, IPV6,
};

struct Ip_info
{
    Ip_family   ip_family;
    std::string ip_string;
};

// TODO: Define exceptions for socket


class Socket
{
public:
    // Create socket from scratch
    Socket(const Sock_type sock_type, const Ip_family ip_family);

    // Create socket object out of socket fd
    Socket(const int sockfd);

    ~Socket();


    // ========== Server methods ========== //
    void bind_to_port(const std::string &port_no);
    void listen_for_conn(const std::size_t &queue_size) const;
    std::pair<Socket, Ip_info> accept_remote_conn() const;


    // ========== Client methods ========== //
    Ip_info connect_to_remote(const std::string &node, 
                              const std::string &port_no) const;

    // ========== Common methods ========== //
    // TODO: Add send() and recv()
    // TODO: Add sendto() and recvfrom() (UDP methods)

    int  get_sockfd() const { return m_sockfd; }
    void close_socket();  // manually closing the socket

private:
    int       m_sockfd    { -1 };  // -1 suggests no socket is created
    Sock_type m_sock_type {};      // TCP or UDP: Connection type socket supports
    Ip_family   m_ip_family   {};     // IPV4 or IPV6: Network type socket supports
};


#endif // SOCKET_H_
