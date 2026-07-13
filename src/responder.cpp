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


std::string
Responder::generate_response()
{
    // If we've already generated a response
    if (!m_response.empty()) {
        return m_response;
    }

    if (!m_request.is_valid) {
        // TODO: Return an error html page
        return m_response;
    }

    const auto response_version{ Http::Version::V1_1 };
    const auto request_method{ m_request.method };
    const std::string resource_path{ process_path(request.path) };

    // Generate the response header
    const auto [response_code, response_header]
        = generate_response_header(resource_path);

    if (response_code != Http::Response_code::OK) {
        // TODO: Return an error html page
    }

    // Generate response body - The actual file contents
    // Only if request method is GET
    std::string response_body{};
    if (request_method == Http::Method::GET) {
        const auto [response_code, body] 
            = generate_response_body(resource_path);

        if (response_code != Http::Response_code::OK) {
            // TODO: Return an error html page
        }
        response_body += body;
    }

    // Get the response line
    auto response_line{ generate_response_line(response_version, 
                                        Http::Response_code::OK)
    };

    // Compose the complete response message
    m_response = compose_response_msg(response_line, response_header,
                                    response_body);

    return m_response;
};


std::string Responder::generate_response_line(const Http::Version version,
                            const Http::Response_code response_code)
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
Responder::generate_response_header(const std::string& resource_path)
{
    std::filesystem::path path{ resource_path };

    // If given path doesn't exist, or if it does but is a 
    // directory instead of a file then error out
    if (!std::filesystem::exists(file_path)
      || std::filesystem::is_directory(file_path)) {
        
        return { Http::Response_code::PAGE_NOT_FOUND, "" };
    }

    auto content_length{ std::filesystem::file_size(file_path) };
    auto content_type{ Http::get_mime_type(path) };

    std::string response_header{ 
        "Content-Length: " + std::to_string(content_length) + '\n'
      + "Content-Type: "   + content_type
    };

    return { Http::Response_code::OK, response_header };
}


[[nodiscard]] std::string
compose_response_msg(const std::string& response_line,
                     const std::string& response_header,
                     const std::string& response_body) const
{
    return (response_line    + "\n"
          + response_header + "\n"
          + "\n"
          + response_body;)
}


std::pair<Http::Response_code, std::string>
Responder::generate_response_body(const std::string& resource_path)
{
    std::ifstream resource_stream{ resource_path };
    if (!resource_stream.is_open()) {
        // TODO: File not opened error handling
    }

    // FIX: Logic to reading whole file
    std::stringstream buffer{};
    buffer << file_obj.rdbuf();

    std::string response_body{ buffer.str() };
    return { Http::Response_code::OK, response_body };
}
