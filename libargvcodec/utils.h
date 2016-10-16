#pragma once

#include <string>
#include <vector>

namespace utils
{

#define SAFE_DELETE(var)        {if (var) delete   var; var = NULL;}
#define SAFE_DELETE_ARRAY(var)  {if (var) delete[] var; var = NULL;}

void strReplace(std::string & iValue, const std::string & iOldStr, const std::string & iNewStr);
void strReplace(std::string & iValue, const char * iOldStr, const char * iNewStr);
std::string uppercase(const std::string & iValue);
std::string uppercase(const char * iValue);

size_t findNumTrailingBackslashes(const char * iValue);

}; //namespace utils
