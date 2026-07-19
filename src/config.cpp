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
#include <fstream>
#include <filesystem>
#include <iostream>
#include <unordered_set>


static std::string
get_config_string(const Config::Type& config)
{
    std::string str{};

    str += "path = "    + config.root_path                    + "\n"
         + "threads = " + std::to_string(config.thread_count) + "\n"
         + "queue = "   + std::to_string(config.queue_size);

    return str;
}


static void
create_default_conf_file(const std::string& default_config_path,
                         const Config::Type& config)
{
    std::ofstream outfile{ default_config_path };
    if (!outfile) {
        // TODO: Error handling
    }

    const auto config_string{ get_config_string(config) };
    outfile << config_string;
}


Config::Type
Config::parse_config_file(const std::string& config_path,
                          const std::string& default_config_path)
{
    // Struct to return
    struct Type config{
        Server::DEFAULT_ROOT_PATH,
        Server::DEFAULT_THREAD_COUNT,
        Server::DEFAULT_QUEUE_SIZE,
    };

    // If given file doesn't exist
    if (!std::filesystem::exists(std::filesystem::path(config_path))) {
        // FIX: config_path comparison with default_config_path is wrong, use filesystem function
        if (config_path == default_config_path) {
            create_default_conf_file(default_config_path, config);
            return config;
        }
        // Provided custom config file doesn't exist
        throw Config::Config_error{ "Config file '" + config_path + "' doesn't exist" };
    }

    const std::string file_contents{ Utils::read_file_content(config_path) };

    // Divide the config into lines
    const auto config_parts{ Parser::tokenize_string(file_contents, "\n") };

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
