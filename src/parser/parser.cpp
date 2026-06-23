#include "parser.h"
#include "http.h"

#include <string>


Http::Request Parser::parse_request(const std::string& msg) const
{
    Http::Request request{};

    if (parse_request_line(request, msg) == -1) {
        return request;
    }

    return request;
}


int parse_request_line(Http::Request& request, const std::string& request_line) const
{
    using TOK_COUNT = 3;

    auto parsed_request{ Parse_helper::tokeize_string(request_line, ' ') };
    if (parsed_request.size() != TOK_COUNT) {
        request.is_valid = false;
        return -1;
    }

    using METHOD_INDEX   = 0;
    using RESOURCE_INDEX = 1;
    using VERSION_INDEX  = 2;

    // Get the method type
    auto& method{ parsed_request.at(METHOD_INDEX) };
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
    request.path = std::move(parsed_request.at(RESOURCE_INDEX));
    // TODO: Add check for resource path syntax

    // Get the http version
    auto& version{ parsed_request.at(VERSION_INDEX) };
    if (version == std::string{ "HTTP/1.0" }) {
        request.version = Http::Version::V1_0;
    }
    else if (version == std::string{ "HTTP/1.1" }) {
        request.version = Http::Version::V1_1;
    }
    else {
        request.is_valid = false;
    }

    return request;
}
