#include "utils.h"

#include <string>
#include <fstream>
#include <sstream>


std::string
Utils::read_file_content(const std::string& file)
{
    std::ifstream stream{ file };
    if (!stream.is_open()) {
        // TODO: File not opened error handling
    }

    std::stringstream buffer{};
    buffer << stream.rdbuf();

    return buffer.str();
}
