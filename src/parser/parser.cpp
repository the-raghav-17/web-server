#include "parser.h"
#include "parser_helper.h"
#include "http.h"

#include <string>


Http::Request Parser::parse_request(const std::string& request_msg) const
{
    auto parsed_request{ Parser_helper::tokenize_string(request_msg, "\r\n") };
    constexpr int REQUEST_LINE_INDEX{ 0 };

    Http::Request request{};

    const std::string& request_line{ parsed_request.at(REQUEST_LINE_INDEX) };
    if (parse_request_line(request_line, request) == -1) {
        return request;
    }

    // TODO: Parsing headers and content

    return request;
}


int Parser::parse_request_line(const std::string& request_line, Http::Request& request) const
{
    constexpr std::size_t TOK_COUNT{ 3 };

    auto parsed_request_line{ Parser_helper::tokenize_string(request_line, " ") };
    if (parsed_request_line.size() != TOK_COUNT) {
        request.is_valid = false;
        return -1;
    }

    constexpr std::size_t METHOD_INDEX{ 0 };
    constexpr std::size_t RESOURCE_INDEX{ 1 };
    constexpr std::size_t VERSION_INDEX{ 2 };

    // Get the method type
    auto& method{ parsed_request_line.at(METHOD_INDEX) };
    if (method == std::string{ "GET" }) {
        request.method = Http::Method::GET;
    }
    else if (method == std::string{ "HEAD" }) {
        request.method = Http::Method::HEAD;
    }
    else if (method == std::string{ "POST" }) {
        request.method = Http::Method::POST;
    }
    else {
        request.is_valid = false;
        return -1;
    }

    // Get the resource path
    request.path = std::move(parsed_request_line.at(RESOURCE_INDEX));
    // TODO: Add check for resource path syntax

    // Get the http version
    auto& version{ parsed_request_line.at(VERSION_INDEX) };
    if (version == std::string{ "HTTP/1.0" }) {
        request.version = Http::Version::V1_0;
    }
    else if (version == std::string{ "HTTP/1.1" }) {
        request.version = Http::Version::V1_1;
    }
    else {
        request.is_valid = false;
        return -1;
    }

    request.is_valid = true;
    return 0;
}
