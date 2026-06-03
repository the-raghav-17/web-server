/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
 */

/*
 * socket_helper.cpp
 *
 * Implementation of socket helper functions,
 * specific to POSIX API.
 */


#include "socket_helper.h"
#include "socket.h"

#include <string>
#include <cassert>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <memory>
#include <arpa/inet.h>


// Convert Sock_type to its POSIX equivalent
// TCP -> SOCK_STREAM; UDP -> SOCK_DGRAM
int Sock_helper::get_sock_type(const Sock_type &sock_type)
{
    assert(sock_type == Sock_type::TCP 
        || sock_type == Sock_type::UDP);

    return sock_type == Sock_type::TCP ? SOCK_STREAM
            : SOCK_DGRAM;
}


int Sock_helper::get_sock_type(const int sockfd)
{
    int level = SOL_SOCKET;
    int optval[1]{};
    socklen_t optlen{ 4 };

    if (getsockopt(sockfd, level, SO_TYPE, optval, &optlen) == -1) {
        return -1;
    }

    return *optval;
}


// Convert Ip_type to its POSIX equivalent
// IPV4 -> AF_INET; IPV6 -> AF_INET6;
int Sock_helper::get_addr_family(const Ip_type &ip_type)
{
    assert(ip_type == Ip_type::IPV4 
        || ip_type == Ip_type::IPV6);

    return ip_type == Ip_type::IPV4 ? AF_INET
        : AF_INET6;
}


// Overloaded variant of the above; does complete opposite
Ip_type Sock_helper::get_addr_family(const int &sa_family_t)
{
    assert(sa_family_t == AF_INET
        || sa_family_t == AF_INET6);

    return sa_family_t == AF_INET ? Ip_type::IPV4
        :  Ip_type::IPV6;
}


int get_addr_family(const int sockfd)
{
    int level = SOL_SOCKET;
    int optval[1]{};
    socklen_t optlen{ 4 };

    if (getsockopt(sockfd, level, SO_DOMAIN, optval, &optlen) == -1) {
        return -1;
    }

    return *optval;
}


std::string Sock_helper::get_ip_string(const sockaddr &addr)
{
    char addr_str[INET6_ADDRSTRLEN];  // buffer to store string
    sa_family_t sa_family { addr.sa_family };  // address family

    // This extracts the address part from sockaddr
    void *src = nullptr;

    if (sa_family == AF_INET) {
        src = &(((struct sockaddr_in *)&addr)->sin_addr);
    }
    else {
        src = &(((struct sockaddr_in6 *)&addr)->sin6_addr);
    }

    inet_ntop(sa_family, src, addr_str, sizeof(addr_str));
    std::string ip_string { addr_str };
    return ip_string;
}


// Return the res linked list which consits of
// all the information reqd to bind a socket.
// To be used when calling bind
Sock_helper::Addrinfo_list
Sock_helper::getaddrinfo_res(const std::string& port_no, const Ip_type ip_type)
{
    struct addrinfo hints {};
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family = Sock_helper::get_addr_family(ip_type);
    hints.ai_flags  = AI_PASSIVE;

    struct addrinfo* res{};
    int status { getaddrinfo(NULL, port_no.c_str(), &hints, &res) };
    if (status == -1) {
        // TODO: Throw exception using gai_strerror()
    }

    Addrinfo_list list{ res };
    return list;
}


// Return the res linked list which consits of
// all the information reqd to bind a socket.
// To be used when calling connect()
Sock_helper::Addrinfo_list
Sock_helper::getaddrinfo_res(const std::string& port_no, const Ip_type ip_type,
                             const std::string& node, Sock_type sock_type)
{
    struct addrinfo hints{};
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family   = Sock_helper::get_addr_family(ip_type);
    hints.ai_socktype = Sock_helper::get_sock_type(sock_type);

    struct addrinfo* res{};
    int status { getaddrinfo(node.c_str(), port_no.c_str(), &hints, &res) };
    if (status == -1) {
        // TODO: Throw exception using gai_strerror()
    }

    Addrinfo_list list{ res };
    return list;
}
