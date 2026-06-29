#ifndef HTTP_H_
#define HTTP_H_


#include <string>


namespace Http
{
    const static std::string VERSION{ "1.0" };

    enum class Method
    {
        GET, HEAD, POST,
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

    struct Response
    {
        Version       version;
        Response_code code;
    };


    std::string get_http_version_string(const Version& version);
    std::string get_code_string(const Response_code& code);
    std::string get_code_msg(const Response_code& code);
};


#endif // HTTP_H_
