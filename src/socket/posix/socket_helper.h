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


namespace Sock_helper
{
    inline int      get_sock_type(const Sock_type &sock_type);
    inline int      get_addr_family(const Ip_type &ip_type);
    inline Ip_type  get_addr_family(const int &sa_family_t);
    std::string     get_ip_string(const sockaddr &addr);
    struct addrinfo *getaddrinfo_res(const Ip_type &ip_type, 
                                    const std::string &port_no);
};


#endif // SOCKET_HELPER_H_
