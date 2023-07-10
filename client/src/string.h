#ifndef STRING_H
#define STRING_H

#pragma once

#include <string>

std::string& Trim(std::string& stringToTrim);
inline void TrimFront(std::string& stringToTrim);
inline void TrimEnd(std::string& stringToTrim);
std::string ToUpper(std::string& stringToUpper);
std::string ToLower(std::string& stringToLower);

#endif // !STRING_H