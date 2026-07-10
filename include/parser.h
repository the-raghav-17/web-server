#ifndef PARSER_H_
#define PARSER_H_


#include "http.h"

#include <string>
#include <vector>


namespace Parser
{
    std::vector<std::string>
        tokenize_string(const std::string& str, const std::string& delim);

    Http::Request
        parse_request(const std::string& request_msg);

    int parse_request_line(const std::string& request_line,
                           Http::Request& request);
};


#endif // PARSER_H_
