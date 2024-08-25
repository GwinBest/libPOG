#ifndef STRING_H
#define STRING_H

#pragma once

#include <string>

namespace StringUtils
{
    std::string& Trim(std::string& stringToTrim);
    void TrimFront(std::string& stringToTrim);
    void TrimEnd(std::string& stringToTrim);

    std::string ToUpper(std::string& stringToUpper);
    std::string ToLower(std::string& stringToLower);
}
#endif // !STRING_H