#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> StringList;

#define SAFE_DELETE(var)        {if (var) delete   var; var = NULL;}
#define SAFE_DELETE_ARRAY(var)  {if (var) delete[] var; var = NULL;}

void strReplace(std::string & iValue, const std::string & iOldStr, const std::string & iNewStr);
void strReplace(std::string & iValue, const char * iOldStr, const char * iNewStr);
bool readTextFile(const char * iPath, StringList & oLines);
size_t findNumTrailingBackslashes(const char * iValue);
