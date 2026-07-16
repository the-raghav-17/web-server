/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * server.h
 *
 * Declaration of server class.
 * The main class that combines all
 * the helper modules to create a 
 * functioning server.
 */


#ifndef SERVER_H_
#define SERVER_H_


#include "socket.h"
#include "http.h"

#include <filesystem>


/**
 * Server_error is the exception class used
 * by the server
 */
class Server_error : public std::runtime_error
{
public:
    Server_error(const std::string& msg):
        runtime_error{ msg }
    {}
};


/**
 * Server class is responsible for connecting with clients,
 * parsing their requests and then providing with a valid
 * response.
 */
class Server
{
public:
    /**
     * Default config settings for the server
     */
    static const     std::string DEFAULT_ROOT_PATH{ "./public" };
    static constexpr int         DEFAULT_THREAD_COUNT{ 5 };
    static constexpr int         DEFAULT_QUEUE_SIZE{ 5 };
    static const     std::string DEFAULT_CONFIG_FILE{ "server.toml" };

    /**
     * Server constructor takes program arguments.
     * It sets up the server configuration based 
     * on the config file (if any)
     */
    Server(int argc, char* argv[]);

    /**
     * Method to start an initialized server.
     * Will setup necessary network services
     * and in a loop, accept client requests,
     * parse them and provide a reply.
     */
    void
      start() const;

private:
    // ----- Data members ----- //

    /**
     * The root directory where all 
     * HTTP content is stored
     */
    std::string m_root_path{};

    /**
     * No. of worker threads in threadpool
     */
    int m_thread_count{};

    /**
     * No. of connections that can be 
     * queued while listening
     */
    int         m_queue_size{};

    // ----- Methods -----//

    /**
     * Takes a socket to remote client, parses
     * their requests and provides a reply.
     */
    void
      handle_client(Socket& remote_socket) const;

    /**
     * Helper, to be removed
     * TODO: Remove request details printer
     */
    void
      print_request_details(const Http::Request& msg_request) const;
};


#endif // SERVER_H_
