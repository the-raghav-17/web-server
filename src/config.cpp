/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * config.cpp
 *
 * Implementation of config functions
 */


#include "config.h"

#include <string>


/*
 * Helper functions will be static here
 */


Config::Type
Config::parse_config_file(const std::string& file_path,
                          const std::string& default_config_file)
{
    // TODO: Logic for file handling
    // ...
    // Assume config file was opened as file_contents
    std::ifstream file{ file_path };
    if (!file) {
        // TODO: Throw error: File error
    }

    // TODO: Read file contents

    // Divide the config into lines
    const auto config_parts{ Parser::tokenize_string(file_contents, "\r\n") };

    if (config_parts.size() > 3) {
        // TODO: Throw error: Excess parameter count... Expected at most 3 got config_parts.size()
    }

    // Struct to return
    struct Type config{
        Server::DEFAULT_ROOT_PATH,
        Server::DEFAULT_THREAD_COUNT,
        SERVER::DEFAULT_QUEUE_SIZE,
    };

    // Set to see if a parameter appears more than once
    std::unordered_set<std::string> set{};

    // Now process each line
    for (const auto& config_line : config_parts) {
        // Divide line based on tokens separated by space character
        auto line_parts{ Parser::tokenize_string(config_line, " ") };

        // Each line must consist of exactly three tokens:
        // 'parameter' '=' 'value'
        if (line_parts.size() != 3) {
            // TODO: Throw error: Excess tokens
        }

        // For now only 3 parameters are defined:
        // - path: root directory which consists all HTTP content
        // - threads: number of threads in the threadpool
        // - queue: size of waiting queue while listening
        //
        // Ex:
        // path = ./content
        // threads = 10
        // queue = 7
        
        if (line_parts.at(1) != "=") {
            // TODO: Throw error: Expected "=" got line_parts.at(1)
        }

        const auto& param{ line_parts.at(0) };
        const auto& value{ line_parts.at(2) };

        // Make sure the parameter only exists once
        if (set.count(param) > 0) {
            // TODO: Throw error: Param path defined twice
        }
        set.insert(param);
        
        if (param == "path") {
            config.root_path = value;
        }
        else if (param == "threads") {
            if (!is_int(value)) {
                // TODO: Throw error: param threads expected int
            }
            config.threads = value;
        }
        else if (param == "queue") {
            if (!is_int(value)) {
                // TODO: Throw error: param queue expected int
            }
            config.queue = value;
        }
        else {
            // TODO: Throw error: Unexpected parameter
        }
    }

    return config;
}
