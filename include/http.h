#ifndef HTTP_H_
#define HTTP_H_


namespace Http
{
    enum class Method
    {
        GET, HEAD, POST,
    };

    struct Request
    {
        bool        is_valid;
        std::string version;
        Method      method;
        std::string path;
    };
};


#undef // HTTP_H_
