/*
 * copyright: Raghav Sharma
 * github:    github.com/the-raghav-17
 * License:   GNU GPLv3
 */

/*
 * main.cpp
 *
 * Starting point of the program.
 * Simply creates a server object and
 * starts the server.
 */


#include "server.h"


int main()
{
    // TODO: Add exceptions for the server
    Server server{};
    server.start();

    return 0;
}
