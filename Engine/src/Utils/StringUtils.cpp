#include "Utils/StringUtils.h"

#include <regex>

namespace StringUtils
{
    std::vector<std::string> Split(const std::string& str, const std::string& delims)
    {
        std::vector<std::string> splitValues;
        const std::regex reg("[" + delims + "]");
        std::copy(
            std::sregex_token_iterator(std::cbegin(str), std::cend(str), reg, -1),
            std::sregex_token_iterator(),
            std::back_inserter(splitValues)
        );
        return splitValues;
    }
}
