/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * http.cpp
 *
 * Definitions of functions in the namespace Http
 */

#include "http.h"
#include "parser.h"

#include <string>


std::string
Http::get_version_string(const Http::Version& version) noexcept
{
    switch (version) {
        case Version::V1_0:
            return "1.0";

        case Version::V1_1:
            return "1.1";
    }
}

std::string
Http::get_code_string(const Http::Response_code& code) noexcept
{
    switch (code) {
        case Response_code::OK:
            return "200";

        case Response_code::BAD_REQUEST:
            return "400";

        case Response_code::UNAUTHORIZED:
            return "401";

        case Response_code::FORBIDDEN:
            return "403";

        case Response_code::PAGE_NOT_FOUND:
            return "404";

        case Response_code::INTERNAL_SERVER_ERR:
            return "500";
    }
}

std::string
Http::get_code_msg(const Response_code& code) noexcept
{
    switch (code) {
        case Response_code::OK:
            return "OK";

        case Response_code::BAD_REQUEST:
            return "Bad Request";

        case Response_code::UNAUTHORIZED:
            return "Unauthorized";

        case Response_code::FORBIDDEN:
            return "Forbidden";

        case Response_code::PAGE_NOT_FOUND:
            return "Page Not Found";

        case Response_code::INTERNAL_SERVER_ERR:
            return "Internal Server Error";
    }
}


std::string
Http::get_mime_type(const std::string& file_path)
{
    // Works by simply tokenizing the path based on '.' character
    // Selects the last token as the file extension.
    //
    // Ex - public/src/.server/img.png
    // Tokenized into: ["public/src/", "server/img", "png"]
    // png will be the extension.

    auto tokens{ Parser::tokenize_string(file_path, ".") };
    auto extension{ tokens.back() };

    // Textual files
    if (extension == "css") {
        return "text/css";
    }
    else if (extension == "html") {
        return "text/html";
    }
    else if (extension == "txt") {
        return "text/plain";
    }
    else if (extension == "js") {
        return "text/javascript";
    }

    // Image files
    else if (extension == "jpg" || extension == "jpeg") {
        return "image/jpeg";
    }
    else if (extension == "png") {
        return "image/png";
    }
    else if (extension == "svg") {
        return "image/svg+xml";
    }
    else if (extension == "webp") {
        return "image/webp";
    }

    // Unknown type
    else {
        return "application/octet-stream";
    }
}
