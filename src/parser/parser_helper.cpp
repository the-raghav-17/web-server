#include "parser_helper.h"

#include <string>
#includde <vector>


std::vector<std::string>
Parser_helper::tokenize_string(const std::string& str, const std::string& delim)
{
    std::vector<std::string> out{};
    std::size_t i{ 0 };

    for (auto found = str.find(delim, 0);
        found != std::string::npos;
        found = str.find(delim, i)) {

        // We're on the delim itself, so don't add it to token
        if (str.substr(i, delim.size()) != delim) {
            out.emplace_back(str, i, found-i);
        }

        i = found + delim.size();
    }
    // If we go across the string (when delim is at end)
    if (i < str.size()) {
        out.emplace_back(str, i);
    }

    return out;
}
