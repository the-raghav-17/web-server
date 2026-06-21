/*
 * Copyright (c) 2026 Raghav Sharma
 * License: GNU GPLv3
 * GitHub: github.com/the-raghav-17
 */

/*
 * main.cpp
 *
 * Starting point of the program.
 * Simply creates a server object and
 * starts the server.
 */


#include "server.h"

#include <iostream>


int main()
{
    try {
        Server server{};
        server.start();
    } catch(...) {
        std::cerr << "main: Uncaught execption recieved...\n";
    }

    return 0;
}
