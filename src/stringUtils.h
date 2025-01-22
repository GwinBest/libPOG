#ifndef STRING_H
#define STRING_H

#include <string>

namespace StringUtils
{
    std::string Trim(const std::string_view stringToTrim);
    std::string TrimFront(const std::string_view stringToTrim);
    std::string TrimEnd(const std::string_view stringToTrim);

    std::string ToUpper(const std::string_view stringToUpper);
    std::string ToLower(const std::string_view stringToLower);
}
#endif // !STRING_H