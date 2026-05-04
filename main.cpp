/*
 * copyright: Raghav Sharma
 * github:    github.com/the-raghav-17
 * License:   GNU GPLv3
 */

/*
 * Starting point of the program.
 * Simply creates a server object and
 * starts the server.
 */


#include "server.h"


int main()
{
    Server server{};
    server.start();

    return 0;
}
