#include "parser.h"
#include "http.h"

#include <string>


Http::Request Parser::parse_request(const std::string& request_msg)
{
    Http::Request request{};
    request.is_valid = false;

    // Tokenize the request message into different lines
    auto request_parts{ tokenize_string(request_msg, "\r\n") };

    // Request line comes first
    const std::string& request_line{ request_parts.front() };
    if (parse_request_line(request_line, request) == -1) {
        return request;
    }

    // TODO: Parsing headers and content

    return request;
}


int Parser::parse_request_line(const std::string& request_line, Http::Request& request)
{
    constexpr std::size_t PARTS_COUNT{ 3 };

    // Tokenize the request line into parts
    auto request_line_parts{ tokenize_string(request_line, " ") };
    if (request_line_parts.size() != PARTS_COUNT) {
        request.is_valid = false;
        return -1;
    }

    constexpr std::size_t METHOD_INDEX{ 0 };
    constexpr std::size_t PATH_INDEX{ 1 };
    constexpr std::size_t VERSION_INDEX{ 2 };

    // Get the method type
    const auto& method{ request_line_parts.at(METHOD_INDEX) };
    if (method == std::string{ "GET" }) {
        request.method = Http::Method::GET;
    }
    else if (method == std::string{ "HEAD" }) {
        request.method = Http::Method::HEAD;
    }
    else {
        request.is_valid = false;
        return -1;
    }

    // Get the resource path
    request.path = std::move(request_line_parts.at(PATH_INDEX));

    // Get the http version
    const auto& version{ request_line_parts.at(VERSION_INDEX) };
    if (version == "HTTP/1.0") {
        request.version = Http::Version::V1_0;
    }
    else if (version == "HTTP/1.1") {
        request.version = Http::Version::V1_1;
    }
    else {
        request.is_valid = false;
        return -1;
    }

    request.is_valid = true;
    return 0;
}


std::vector<std::string>
Parser::tokenize_string(const std::string& str, const std::string& delim)
{
    std::vector<std::string> out{};
    std::size_t i{ 0 };

    for (auto found = str.find(delim, 0);
        found != std::string::npos;
        found = str.find(delim, i)) {

        // We're on the delim itself, so don't add it to token
        if (str.substr(i, delim.size()) != delim) {
            out.emplace_back(str, i, found-i);
        }

        i = found + delim.size();
    }
    // If we go across the string (when delim is at end)
    if (i < str.size()) {
        out.emplace_back(str, i);
    }

    return out;
}
