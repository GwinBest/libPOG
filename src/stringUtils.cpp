#include "stringUtils.h"

namespace StringUtils
{
    std::string Trim(const std::string_view stringToTrim)
    {
        std::string resultString = stringToTrim.data();

        resultString = TrimFront(resultString);
        resultString = TrimEnd(resultString);

        return resultString;
    }

    std::string TrimFront(const std::string_view stringToTrim)
    {
        std::string resultString = stringToTrim.data();

        resultString.erase(resultString.begin(), std::find_if(
            resultString.begin(),
            resultString.end(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }));

        return resultString;
    }

    std::string TrimEnd(const std::string_view stringToTrim)
    {
        std::string resultString = stringToTrim.data();

        resultString.erase(std::find_if(
            resultString.rbegin(),
            resultString.rend(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }).base(), resultString.end());

        return resultString;
    }

    std::string ToUpper(const std::string_view stringToUpper)
    {
        std::string resultString = stringToUpper.data();

        for (auto& ch : resultString)
            ch = toupper(ch);

        return resultString;
    }

    std::string ToLower(const std::string_view stringToLower)
    {
        std::string resultString = stringToLower.data();

        for (auto& ch : resultString)
            ch = tolower(ch);

        return resultString;
    }
}