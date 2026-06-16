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
#include <iostream>
#include <arpa/inet.h>


int Sock_helper::get_sock_type(const int sockfd)
{
    int level = SOL_SOCKET;
    int optval[1]{};
    socklen_t optlen{ 4 };

    if (getsockopt(sockfd, level, SO_TYPE, optval, &optlen) == -1) {
        const std::string err_msg{ strerror(errno) };
        throw Socket_error{ "getsockopt: " + err_msg };
    }

    return *optval;
}


int Sock_helper::get_ip_family(const int sockfd)
{
    int level = SOL_SOCKET;
    int optval[1]{};
    socklen_t optlen{ 4 };

    if (getsockopt(sockfd, level, SO_DOMAIN, optval, &optlen) == -1) {
        const std::string err_msg{ strerror(errno) };
        throw Socket_error{ "getsockopt: " + err_msg };
    }

    return *optval;
}


std::string Sock_helper::get_ip_string(const sockaddr &addr)
{
    char addr_str[INET6_ADDRSTRLEN];  // buffer to store string
    sa_family_t sa_family { addr.sa_family };  // address family

    if (sa_family != AF_INET && sa_family != AF_INET6) {
        throw Socket_error{ "get_ip_string: Invalid IP family" };
    }

    // This extracts the address part from sockaddr
    void *src = nullptr;

    if (sa_family == AF_INET) {
        src = &(((struct sockaddr_in *)&addr)->sin_addr);
    } else {
        src = &(((struct sockaddr_in6 *)&addr)->sin6_addr);
    }

    if (inet_ntop(sa_family, src, addr_str, sizeof(addr_str)) == nullptr) {
        const std::string err_msg{ strerror(errno) };
        throw Socket_error{ "inet_ntop: " + err_msg };
    }

    std::string ip_string{ addr_str };
    return ip_string;
}


// Return the res linked list which consits of
// all the information reqd to bind a socket.
// To be used when calling bind
Sock_helper::Addrinfo_list
Sock_helper::getaddrinfo_list(const std::string& port_no, const int ip_family)
{
    struct addrinfo hints{};
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family = ip_family;
    hints.ai_flags  = AI_PASSIVE;

    struct addrinfo* res{};
    int status{ getaddrinfo(NULL, port_no.c_str(), &hints, &res) };
    if (status != 0) {
        const std::string err_msg{ gai_strerror(status) };
        throw Socket_error{ "getaddrinfo: " + err_msg };
    }

    Addrinfo_list list{ res };
    return list;
}


// Return the res linked list which consits of
// all the information reqd to bind a socket.
// To be used when calling connect()
Sock_helper::Addrinfo_list
Sock_helper::getaddrinfo_list(const std::string& port_no, const int ip_family,
                             const std::string& node, int sock_type)
{
    struct addrinfo hints{};
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family   = ip_family;
    hints.ai_socktype = sock_type;

    struct addrinfo* res{};
    int status{ getaddrinfo(node.c_str(), port_no.c_str(), &hints, &res) };
    if (status != 0) {
        const std::string err_msg{ gai_strerror(status) };
        throw Socket_error{ "getaddrinfo: " + err_msg };
    }

    Addrinfo_list list{ res };
    return list;
}
