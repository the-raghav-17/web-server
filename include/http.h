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

    struct Request
    {
        bool        is_valid;
        Version     version;
        Method      method;
        std::string path;
    };
};


#endif // HTTP_H_
