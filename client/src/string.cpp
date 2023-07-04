#include "string.h"

std::string Trim(std::string stringToTrim)
{
	TrimFront(stringToTrim);
	TrimEnd(stringToTrim);

	return stringToTrim;
} 

inline void TrimFront(std::string& stringToTrim)
{
	stringToTrim.erase(stringToTrim.begin(), std::find_if(stringToTrim.begin(), stringToTrim.end(), [](unsigned char ch) {
		return !std::isspace(ch);}));
}

inline void TrimEnd(std::string& stringToTrim)
{
	stringToTrim.erase(std::find_if(stringToTrim.rbegin(), stringToTrim.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), stringToTrim.end());
}

std::string ToUpper(std::string stringToUpper)
{
	for (auto& ch : stringToUpper)
	{
			ch = toupper(ch);
	}
	
	return stringToUpper;
}

std::string ToLower(std::string stringToLower)
{
	for (auto& ch : stringToLower)
	{
			ch = tolower(ch);
	}
	
	return stringToLower;
}


