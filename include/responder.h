/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/**
 * responder.h
 *
 * Declaration of the responder class which
 * is responsible for providing response
 * facilities like generating a response to
 * an HTTP request
 */


#ifndef RESPONDER_H_
#define RESPONDER_H_


#include "http.h"

#include <string>
#include <utility>


/**
 * Responder class is responsible for parsing the request message
 * and then generating a valid response for that message.
 * It will produce a response string for a particular request message
 * and for that it will also read the file contents requested.
 */
class Responder
{
public:
    /**
     * Responder constructor takes the request message
     * and the name of the root directory where all the
     * HTTP content is stored
     */
    Responder(const std::string& request_msg,
            const std::string& root_path);

    /**
     * Generates a response message that can be 
     * sent back to the client
     */
    [[nodiscard]] std::string
        get_response();

    /**
     * Get the parsed request message.
     * Returns the struct which has all the details of
     * the parsed request message.
     */
    [[nodiscard]] Http::Request
        get_parsed_request() const noexcept {
            return m_request;
        }

private:
    /**
     * The root directory where all the 
     * HTTP content is stored
     */
    std::string m_root_path{};

    /**
     * The original request message sent
     * by the client
     */
    std::string m_request_msg{};

    /**
     * The parsed request message
     */
    Http::Request m_request{};

    /**
     * The response message generated
     * by the responder
     */
    std::string m_response_msg{};

    // ----- Methods -----//

    /**
     * Generates and returns the response line string.
     *
     * Takes the HTTP version and the response
     * code of the response.
     */
    [[nodiscard]] std::string
        get_response_line(const Http::Version version,
                          const Http::Response_code response_code) const;

    /**
     * Generates response header for a particular
     * resource requested.
     * Takes the path to resource as parameter.
     * Returns a pair of response code and the actual
     * response header string.
     * On success, response code is set to OK. Otherwise,
     * to any other value to represent the error.
     */
    [[nodiscard]] std::pair<Http::Response_code, std::string>
        get_response_header(const std::string& resource_path) const;

    /**
     * Generates response body for a particular
     * resource requested.
     * Takes the path to resource as parameter.
     * Returns a pair of response code and the actual
     * response body string.
     * On success, response code is set to OK. Otherwise,
     * to any other value to represent the error.
     */
    [[nodiscard]] std::pair<Http::Response_code, std::string>
        get_response_body(const std::string& resource_path) const;

    /*
     * Compose a full HTTP response message.
     * Given the response line, response header
     * and response body, compose a full HTTP response
     * message.
     * Just adds necessary new-lines and doesn't perform
     * any kind of syntactic checks on the provided strings
     */
    [[nodiscard]] std::string
        compose_response_msg(const std::string& response_line,
                             const std::string& response_header,
                             const std::string& response_body) const;

    // TODO: Add comment for get_error_page
    [[nodiscard]] std::string
        get_error_page(const Http::Response_code response_code,
                                const Http::Version http_version) const;

    /**
     * Modifies the path of resource requested
     * by adding the root path string in front
     * and making any other changes required
     */
    [[nodiscard]] std::string 
        process_path(const std::string& path) const;
};


#endif // RESPONDER_H_
