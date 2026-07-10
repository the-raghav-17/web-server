#ifndef PARSER_H_
#define PARSER_H_


#include "http.h"

#include <string>
#include <vector>


class Parser
{
public:
    Http::Request parse_request(const std::string& request_msg) const;

    static std::vector<std::string>
        tokenize_string(const std::string& str, const std::string& delim);

private:
    int parse_request_line(const std::string& request_line, Http::Request& request) const;
};


#endif // PARSER_H_
