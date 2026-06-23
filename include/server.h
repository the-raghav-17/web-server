/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
 */

/*
 * server.h
 *
 * Declaration of server class.
 */


#ifndef SERVER_H_
#define SERVER_H_


class Server
{
public:
    // TODO: Add server constructor that enables signal handling andd initialized invariants
    void start() const;

private:
    void handle_client(Socket& remote_socket) const;
};


#endif // SERVER_H_
