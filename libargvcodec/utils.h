#pragma once

#include <string>
#include <vector>

namespace libargvcodec
{
namespace utils
{

void strReplace(std::string & iValue, const std::string & iOldStr, const std::string & iNewStr);
void strReplace(std::string & iValue, const char * iOldStr, const char * iNewStr);
size_t findNumTrailingBackslashes(const char * iValue);
std::string uppercase(const std::string & iValue);
std::string uppercase(const char * iValue);

}; //namespace utils
}; //namespace libargvcodec
