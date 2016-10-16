#pragma once

#include <string>
#include <vector>

namespace utils
{

void strReplace(std::string & iValue, const std::string & iOldStr, const std::string & iNewStr);
void strReplace(std::string & iValue, const char * iOldStr, const char * iNewStr);
std::string uppercase(const std::string & iValue);
std::string uppercase(const char * iValue);

size_t findNumTrailingBackslashes(const char * iValue);

}; //namespace utils
