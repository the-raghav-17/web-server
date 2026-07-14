/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/**
 * responder.cpp
 *
 * Definition of the responder class
 */


#include "responder.h"
#include "parser.h"

#include <string>
#include <utility>
#include <filesystem>
#include <fstream>
#include <sstream>


Responder::Responder(const std::string& request_msg, 
                    const std::string& root_path) noexcept
    :m_root_path{ root_path },
     m_request_msg{ request_msg }
{
     m_request = Parser::parse_request(request_msg);
}


std::string
Responder::get_response()
{
    // If we've already generated a response
    if (!m_response_msg.empty()) {
        return m_response_msg;
    }

    constexpr auto response_version{ Http::Version::V1_1 };

    if (!m_request.is_valid) {
        m_response_msg = get_error_page(Http::Response_code::BAD_REQUEST,
                                response_version);
        return m_response_msg;
    }

    const auto        request_method{ m_request.method };
    const std::string resource_path{ process_path(m_request.path) };

    // Generate the response header
    const auto [response_code, response_header]
        = get_response_header(resource_path);

    if (response_code != Http::Response_code::OK) {
        m_response_msg = get_error_page(response_code,
                                response_version);
        return m_response_msg;
    }

    // Generate response body - The actual file contents
    // Only if request method is GET
    std::string response_body{};
    if (request_method == Http::Method::GET) {
        const auto [response_code, body] 
            = get_response_body(resource_path);

        if (response_code != Http::Response_code::OK) {
            m_response_msg = get_error_page(response_code,
                                    response_version);
            return m_response_msg;
        }
        response_body += body;
    }

    // Get the response line
    const auto response_line{ get_response_line(response_version, 
                                        Http::Response_code::OK) };

    // Compose the complete response message
    m_response_msg = compose_response_msg(response_line, response_header,
                                    response_body);

    return m_response_msg;
};


std::string
Responder::get_response_line(const Http::Version version,
                            const Http::Response_code response_code) const
{
    std::string response_line{ "HTTP/" };
    response_line += Http::get_version_string(version);

    response_line += " ";

    response_line += Http::get_code_string(response_code);
    response_line += " ";
    response_line += Http::get_code_msg(response_code);

    return response_line;
}


std::pair<Http::Response_code, std::string>
Responder::get_response_header(const std::string& resource_path) const
{
    std::filesystem::path path{ resource_path };

    // If given path doesn't exist, or if it does but is a 
    // directory instead of a file, then error out
    if (!std::filesystem::exists(path)
      || std::filesystem::is_directory(path)) {
        
        return { Http::Response_code::PAGE_NOT_FOUND, "" };
    }

    auto content_length{ std::filesystem::file_size(path) };
    auto content_type{ Http::get_mime_type(resource_path) };

    std::string response_header{ 
        "Content-Length: " + std::to_string(content_length) + '\n'
      + "Content-Type: "   + content_type
    };

    return { Http::Response_code::OK, response_header };
}


std::pair<Http::Response_code, std::string>
Responder::get_response_body(const std::string& resource_path) const
{
    std::ifstream resource_stream{ resource_path };
    if (!resource_stream.is_open()) {
        // TODO: File not opened error handling
    }

    // FIX: Logic to reading whole file
    std::stringstream buffer{};
    buffer << resource_stream.rdbuf();

    std::string response_body{ buffer.str() };
    return { Http::Response_code::OK, response_body };
}


std::string
Responder::compose_response_msg(const std::string& response_line,
                             const std::string& response_header,
                             const std::string& response_body) const
{
    return response_line    + "\n"
          + response_header + "\n"
          + "\n"
          + response_body;
}


std::string
Responder::get_error_page(const Http::Response_code response_code,
                        const Http::Version version) const
{
    // Generate response line, response body and response header based
    // on the error occured and simply return the composed page

    const auto response_line{ get_response_line(version, response_code) };

    const auto code_str{ Http::get_code_string(response_code) };
    const auto code_msg{ Http::get_code_msg(response_code) };

    const std::string response_body{
        "<html>"
            "<head>"
                "<meta charset=\"UTF-8\">"
                "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                "<title>" + code_msg + "</title>"
            "</head>"

            "<body>"
                "<center><h1>" + code_str + " " + code_msg + "</h1></center>"
            "</body>"
        "</html>"
    };

    const std::string response_header{
        "Content-Type: text/html" "\n"
        "Content-Length: " + std::to_string(response_body.size())
    };

    return compose_response_msg(response_line, response_header,
                                response_body);
}


std::string 
Responder::process_path(const std::string& path) const
{
    std::string processed_path{ path };
    if (processed_path == "/") {
        processed_path += "index.html";
    }

    processed_path = m_root_path + processed_path;
    return processed_path;
}
