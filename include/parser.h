#ifndef PARSER_H_
#define PARSER_H_


#include "http.h"

#include <string>


class Parser
{
public:
    Http::Request parse_request(const std::string& request_msg) const;

private:
    int parse_request_line(const std::string& request_line, Http::Request& request) const;
};


#endif // PARSER_H_
