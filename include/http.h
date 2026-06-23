#ifndef HTTP_H_
#define HTTP_H_


namespace Http
{
    enum class Method
    {
        GET, HEAD, POST,
    };

    enum class Version
    {
        V1_1, V1_0,
    }:

    struct Request
    {
        bool        is_valid;
        Version     version;
        Method      method;
        std::string path;
    };
};


#undef // HTTP_H_
