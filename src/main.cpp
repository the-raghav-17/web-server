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


int main(int argc, char* argv[])
{
    // FIX: Giving root path argument

    const std::string root_path{ "./public/" };
    //
    // if (argc > 2) {
    //     std::cerr << "Usage: " << argv[0] << " [root-path]\n";
    //     return 1;
    // }
    // else if (argc == 2) {
    //     root_path += std::string(argv[1]);
    // }
    // else {
    //     root_path += std::string("./public/");
    // }

    try {
        Server server{ root_path };
        server.start();
    }
    catch (Server_error& err) {
        std::cerr << "Error: Server: " + std::string{ err.what() } << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "Some unknown exception caught...\n";
        return 1;
    }

    return 0;
}
