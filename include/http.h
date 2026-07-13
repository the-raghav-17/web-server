/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

#ifndef HTTP_H_
#define HTTP_H_


#include <string>


namespace Http
{
    enum class Method
    {
        GET, HEAD,
    };

    enum class Version
    {
        V1_1, V1_0,
    };

    enum class Response_code
    {
        OK = 200,

        BAD_REQUEST    = 400,
        UNAUTHORIZED   = 401,
        FORBIDDEN      = 403,
        PAGE_NOT_FOUND = 404,
        
        INTERNAL_SERVER_ERR = 500,
    };

    struct Request
    {
        bool        is_valid;
        Version     version;
        Method      method;
        std::string path;
    };

    std::string get_version_string(const Version& version);

    std::string get_code_string(const Response_code& code);
    std::string get_code_msg(const Response_code& code);

    std::string get_mime_type(const std::string& file_path);
};


#endif // HTTP_H_
