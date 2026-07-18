/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * main.cpp
 *
 * Starting point of the program.
 * Simply creates a server object and
 * starts the server.
 */


#include "server.h"
#include "config.h"

#include <iostream>


int main(int argc, char* argv[])
{
    try {
        Server server{ argc, argv };
        server.start();
    }
    catch (Server_error& err) {
        std::cerr << "Error: Server: " << err.what() << '\n';
        return 1;
    }
    catch (Config::Config_error& err) {
        std::cerr << "Error: Config: " << err.what() << '\n';
    }
    catch (...) {
        std::cerr << "Some unknown exception caught...\n";
        return 1;
    }

    return 0;
}
