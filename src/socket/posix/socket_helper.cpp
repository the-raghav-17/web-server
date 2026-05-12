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
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


// Convert Sock_type to its POSIX equivalent
// TCP -> SOCK_STREAM; UDP -> SOCK_DGRAM
inline int Sock_helper::get_sock_type(const Sock_type &sock_type)
{
    assert(sock_type == Sock_type::TCP 
        || sock_type == Sock_type::UDP);

    return sock_type == Sock_type::TCP ? SOCK_STREAM
            : SOCK_DGRAM;
}


// Convert Ip_type to its POSIX equivalent
// IPV4 -> AF_INET; IPV6 -> AF_INET6;
inline int Sock_helper::get_addr_family(const Ip_type &ip_type)
{
    assert(ip_type == Ip_type::IPV4 
        || ip_type == Ip_type::IPV6);

    return ip_type == Ip_type::IPV4 ? AF_INET
        : AF_INET6;
}


// Overloaded variant of the above; does complete opposite
inline Ip_type Sock_helper::get_addr_family(const int &sa_family_t)
{
    assert(sa_family_t == AF_INET
        || sa_family_t == AF_INET6);

    return sa_family_t == AF_INET ? IPV4
        :  IPV6;
}


std::string Sock_helper::get_ip_string(const sockaddr &addr)
{
    char addr_str[INET6_ADDRSTRLEN];  // buffer to store string
    sa_family_t sa_family { addr.sa_family };  // address family

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
struct addrinfo *Sock_helper::getaddrinfo_res(const Ip_type &ip_type,
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
