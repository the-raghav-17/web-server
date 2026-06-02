/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
 */

/*
 * socket_helper.h
 *
 * Helper functions for socket.
 * Specific to POSIX API.
 */


#ifndef SOCKET_HELPER_H_
#define SOCKET_HELPER_H_


#include "socket.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


namespace Sock_helper
{
    int              get_sock_type(const Sock_type &sock_type);
    int              get_sock_type(const int sockfd);   // socket type of socket

    int              get_addr_family(const Ip_type &ip_type);
    Ip_type          get_addr_family(const int &sa_family_t);
    int              get_addr_family(const int sockfd);  // Address family of socket

    std::string      get_ip_string(const struct sockaddr &addr);

    struct addrinfo *getaddrinfo_res(const std::string &port_no,
                        const Ip_type &ip_type);

    struct addrinfo *getaddrinfo_res(const std::string &port_no,
                        const Ip_type &ip_type, const std::string &node,
                        const Sock_type &sock_type = Sock_type::TCP);
};


#endif // SOCKET_HELPER_H_
