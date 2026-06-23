#include "parser.h"
#include "http.h"

#include <string>


Parser::Parse_request(const std::string& msg) const
{
    Http::Request request{};

    std::vector<std::string> parsed_request{};
    std::size_t i{ 0 };

    // Parse Method and resource path
    for (int j = 0; j < 2; j++) {
        auto found{ msg.find(' ', i) };
        if (found == std::string::npos) {
            // TODO: Handle request parsing error
            request.is_valid = false;
        }
        parsed_request.emplace_back(msg, i, found-i);
        i = found + 1;
    }

    // Parse http version
    parsed_request.emplace_back(msg, i);

    // Get the method type
    auto& method{ parsed_request.at(0) };
    if (method == std::string("GET")) {
        request.method = Http::Method::GET;
    } else if (method == std::string("HEAD")) {
        request.method = Http::Method::HEAD;
    } else if (method == std::string("POST")) {
        request.method = Http::Method::POST;
    } else {
        request.is_valid = false;
    }

    // Get the resource path
    request.path = std::move(parsed_request.at(1));

    // Get the http version
    auto& version{ parsed_request.at(2) };
    if (version == std::string("HTTP/1.0\n")) {
        request.version = Http::Version::V1_0;
    } else if (version == std::string("HTTP/1.1\n")) {
        request.version = Http::Version::V1_1;
    } else {
        request.is_valid = false;
    }

    return request;
}
