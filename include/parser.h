/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * parser.h
 *
 * Declaration of various parsing utilities
 * like request parsing, request line parsing
 * and string tokenization
 */


#ifndef PARSER_H_
#define PARSER_H_


#include "http.h"

#include <string>
#include <vector>


/**
 * The Parser namespaces provides helper functions
 * to parse HTTP requests and to tokenize strings
 */
namespace Parser
{
    /**
     * Parser function to parse complete HTTP requests.
     * Calls other helper functions to parse request
     * parts like request line and headers.
     * Takes the request message string to parse.
     * Returns a struct of parsed request.
     */
    [[nodiscard]] Http::Request
        parse_request(const std::string& request_msg);

    /**
     * Parser function to parse HTTP request lines.
     * Takes the request line string to parse and the
     * reference to HTTP request struct where to store
     * the parsed results.
     * Returns -1 to showcase invalid request. Else 0
     */
    [[nodiscard]] int
        parse_request_line(const std::string& request_line,
                           Http::Request& request);


    /**
     * Helper function to tokenize a string.
     * Takes the string to tokenize and the delimiter.
     * The string is divided based on the delim and the
     * result is returned as a vector of strings.
     */
    [[nodiscard]] std::vector<std::string>
        tokenize_string(const std::string& str, const std::string& delim);
};


#endif // PARSER_H_
