#ifndef STRING_H
#define STRING_H

#include <string>

class StringUtils {
public:
    static std::string Trim(const std::string_view stringToTrim);
    static std::string TrimFront(const std::string_view stringToTrim);
    static std::string TrimEnd(const std::string_view stringToTrim);

    static std::string ToUpper(const std::string_view stringToUpper);
    static std::string ToLower(const std::string_view stringToLower);
};

#endif // !STRING_H