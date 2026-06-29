#include "http.h"

#include <string>


std::string Http::get_http_version_string(const Http::Version& version)
{
    switch (version) {
        case Http::Version::V1_0:
            return "1.0";

        case Http::Version::V1_1:
            return "1.1";
    }
}

std::string Http::get_code_string(const Http::Response_code& code)
{
    switch (code) {
        case Http::Response_code::OK:
            return "200";

        case Http::Response_code::BAD_REQUEST:
            return "400";

        case Http::Response_code::UNAUTHORIZED:
            return "401";

        case Http::Response_code::FORBIDDEN:
            return "403";

        case Http::Response_code::PAGE_NOT_FOUND:
            return "404";

        case Http::Response_code::INTERNAL_SERVER_ERR:
            return "500";
    }
}

std::string Http::get_code_msg(const Response_code& code)
{
    switch (code) {
        case Http::Response_code::OK:
            return "OK";

        case Http::Response_code::BAD_REQUEST:
            return "Bad Request";

        case Http::Response_code::UNAUTHORIZED:
            return "Unauthorized";

        case Http::Response_code::FORBIDDEN:
            return "Forbidden";

        case Http::Response_code::PAGE_NOT_FOUND:
            return "Page Not Found";

        case Http::Response_code::INTERNAL_SERVER_ERR:
            return "Internal Server Error";
    }
}
