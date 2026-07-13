/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

#include "responder.h"


Responder::Responder(const std::string& request_msg, 
                    const std::string& root_path):
    m_request_msg{ request_msg },
    m_root_path{ root_path }
{
    m_request{ Parser::parse_request(request_msg) };
}


std::string Responder::generate_response()
{
    // If we've already generated a response
    if (!m_response.empty()) {
        return m_response;
    }

    if (!m_request.is_valid) {
        // TODO: Return an error html page
    }

    const auto version{ Http::Version::V1_0 };
    const auto request_method{ m_request.method };
    const std::string path{ process_path(request.path) };

    // Generate the response header
    const auto [response_code, response_header]{
        generate_response_header(path)
    };
    if (response_code != Http::Response_code::OK) {
        // TODO: Return an error html page
    }

    // Generate response body - The actual file contents
    // Only if request method is GET
    std::string response_body{};
    if (request_method == Http::Method::GET) {
        const auto [response_code, body]{
            generate_response_body(path);
        }
        if (response_code != Http::Response_code::OK) {
            // TODO: Return an error html page
        }
        response_body += body;
    }

    // Get the response line
    auto response_line{ generate_response_line(
            version, Http::Response_code::OK)
    };

    m_response = response_line   + "\n"
               + response_header + "\n"
               + "\n"
               + "response_body";

    return m_response;
};
