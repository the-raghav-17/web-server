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
#include <memory>


namespace Sock_helper
{
    struct List_deleter
    {
        void operator()(struct addrinfo* list)
        {
            freeaddrinfo(list);
        }
    };

    using Addrinfo_list = std::unique_ptr<struct addrinfo, List_deleter>;
    
    int         convert_sock_type(const Sock_type sock_type);
    Sock_type   convert_sock_type(const int sock_type);
    int         get_sock_type(const int sockfd);   // socket type of socket

    int         convert_ip_family(const Ip_family ip_family);
    Ip_family   convert_ip_family(const int sa_family_t);
    Ip_family   get_ip_family(const struct sockaddr& addr);
    int         get_ip_family(const int sockfd);  // Address family of socket
    std::string get_ip_string(const struct sockaddr& addr);

    Addrinfo_list
    getaddrinfo_list(const std::string& port_no, const Ip_family ip_family);

    Addrinfo_list
    getaddrinfo_list(const std::string& port_no, const Ip_family ip_family,
                    const std::string& node, Sock_type sock_type=Sock_type::TCP);
};


#endif // SOCKET_HELPER_H_
