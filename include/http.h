/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

/*
 * http.h
 *
 * Declaration of HTTP utilities
 * like methods, version, response code and
 * helper functions
 */


#ifndef HTTP_H_
#define HTTP_H_


#include <string>
#include <optional>


namespace Http
{
    // ----- Types ----- //

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

    // ----- Functions ----- //

    /** FIX: Improve this comment
     * Takes a string, and returns the HTTP
     * method from it. Returns std::nullopt
     * if string isn't matched with any methods.
     */
    [[nodiscard]] std::optional<Http::Method>
        get_method_from_str(const std::string& method_str) noexcept;

    /** FIX: Improve this comment
     * Takes a string, and returns the HTTP
     * version from it. Returns std::nullopt
     * if string isn't matched with any version.
     */
    [[nodiscard]] std::optional<Http::Version>
        get_version_from_str(const std::string& version_str) noexcept;

    /**
     * Takes Http::Version type and returns
     * the version string.
     * Like: Http::Version::V1_1 -> "1.1"
     */
    [[nodiscard]] std::string 
        get_version_string(const Version& version) noexcept;

    /**
     * Takes Http::Response_code type and returns
     * the code string.
     * Like: Http::Response_code::OK -> "OK"
     */
    [[nodiscard]] std::string 
        get_code_string(const Response_code& code) noexcept;

    /**
     * Takes Http::Response_code type and
     * returns the message associated with 
     * the code.
     * Like: Http::Response_code::BAD_REQUEST -> "Bad Request"
     */
    [[nodiscard]] std::string
        get_code_msg(const Response_code& code) noexcept;

    /**
     * Takes the path to a file and returns the
     * MIME string associated with file type.
     * Doesn't do any kind of check on whether
     * the file exists or not.
     * For files of unknown type, returns "application/octet-stream"
     */
    [[nodiscard]] std::string
        get_mime_type(const std::string& file_path);
};


#endif // HTTP_H_
