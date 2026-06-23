#ifndef PARSER_H_
#define PARSER_H_


#include "http.h"

#include <string>


class Parser
{
public:
    Http::Request parse_request(const std::string& msg) const;

private:
    
};


#undef // PARSER_H_
