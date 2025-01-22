#include "stringUtils.h"

#include <algorithm>

std::string StringUtils::Trim(const std::string_view stringToTrim) {
    std::string resultString = stringToTrim.data();

    resultString = TrimFront(resultString);
    resultString = TrimEnd(resultString);

    return resultString;
}

std::string StringUtils::TrimFront(const std::string_view stringToTrim) {
    std::string resultString = stringToTrim.data();

    resultString.erase(
        resultString.begin(),
        std::find_if(
            resultString.begin(),
            resultString.end(),
            [](unsigned char ch) { return !std::isspace(ch); }
        )
    );

    return resultString;
}

std::string StringUtils::TrimEnd(const std::string_view stringToTrim) {
    std::string resultString = stringToTrim.data();

    resultString.erase(
        std::find_if(
            resultString.rbegin(),
            resultString.rend(),
            [](unsigned char ch) { return !std::isspace(ch); }
        ).base(),
        resultString.end()
    );

    return resultString;
}

std::string StringUtils::ToUpper(const std::string_view stringToUpper) {
    std::string resultString = stringToUpper.data();

    for (auto& ch : resultString)
        ch = toupper(ch);

    return resultString;
}

std::string StringUtils::ToLower(const std::string_view stringToLower) {
    std::string resultString = stringToLower.data();

    for (auto& ch : resultString)
        ch = tolower(ch);

    return resultString;
}