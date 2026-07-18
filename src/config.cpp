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
#include "server.h"
#include "utils.h"
#include "parser.h"

#include <string>
#include <filesystem>
#include <unordered_set>


static void
fill_config(Config::Type& config, const std::string& param)
{
}


Config::Type
Config::parse_config_file(const std::string& file_path,
                          const std::string& default_config_file)
{
    // Struct to return
    struct Type config{
        Server::DEFAULT_ROOT_PATH,
        Server::DEFAULT_THREAD_COUNT,
        Server::DEFAULT_QUEUE_SIZE,
    };

    if (!std::filesystem::exists(std::filesystem::path(file_path))) {
        if (file_path == default_config_file) {
            // TODO: Create default file
            return config;
        }
        // Provided custom config file doesn't exist
        throw Config::Config_error{ "Config file '" + file_path + "' doesn't exist" };
    }

    const std::string file_contents{ Utils::read_file_content(file_path) };

    // Divide the config into lines
    const auto config_parts{ Parser::tokenize_string(file_contents, "\r\n") };

    if (config_parts.size() > 3) {
        throw Config::Config_error{ "Excess config parameters..." 
                                    "expected at most 3, got "
                                  + std::to_string(config_parts.size()) 
        };
    }

    // Set to see if a parameter appears more than once
    std::unordered_set<std::string> set{};

    // Now process each line
    for (const auto& config_line : config_parts) {
        // Divide line based on tokens separated by space character
        auto line_parts{ Parser::tokenize_string(config_line, " ") };

        // Each line must consist of exactly three tokens:
        // 'parameter' '=' 'value'
        if (line_parts.size() != 3) {
            throw Config::Config_error{ "Excess tokens in config parameter" };
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
            throw Config::Config_error{ "Expected '=', got " + line_parts.at(1) };
        }

        const auto& param{ line_parts.at(0) };
        const auto& value{ line_parts.at(2) };

        // Make sure the parameter only exists once
        if (set.count(param) > 0) {
            throw Config::Config_error{ "Parameter '" + param + "' defined twice" };
        }
        set.insert(param);
        
        if (param == "path") {
            config.root_path = value;
        }
        else if (param == "threads") {
            auto int_val{ std::stoi(value) };
            // if (!int_val) {
            //     // TODO: Throw error: param threads expected int
            // }
            config.thread_count = int_val;
        }
        else if (param == "queue") {
            auto int_val{ std::stoi(value) };
            // if (!int_val) {
            //     // TODO: Throw error: param threads expected int
            // }
            config.queue_size = int_val;
        }
        else {
            throw Config::Config_error{ "Unexpected parameter '" + param + "'" };
        }
    }

    return config;
}
