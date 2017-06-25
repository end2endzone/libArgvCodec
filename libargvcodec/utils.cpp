#include "utils.h"

namespace libargvcodec
{
namespace utils
{

void strReplace(std::string & iValue, const std::string & iOldStr, const std::string & iNewStr)
{
  size_t pos = 0;
  while((pos = iValue.find(iOldStr, pos)) != std::string::npos)
  {
    iValue.replace(pos, iOldStr.length(), iNewStr);
    pos += iNewStr.length();
  }
}

void strReplace(std::string & iValue, const char * iOldStr, const char * iNewStr)
{
  size_t oldLength = std::string(iOldStr).length();
  size_t newLength = std::string(iNewStr).length();
  size_t pos = 0;
  while((pos = iValue.find(iOldStr, pos)) != std::string::npos)
  {
    iValue.replace(pos, oldLength, iNewStr);
    pos += newLength;
  }
}

size_t findNumTrailingBackslashes(const char * iValue)
{
  if (iValue == NULL)
    return 0;

  size_t numTrailingBackslashes = 0;
  size_t len = std::string(iValue).size();
  for(size_t i=0; i<len; i++)
  {
    char c = iValue[i];
    if (c == '\\')
      numTrailingBackslashes++;
    else
      numTrailingBackslashes = 0;
  }
  return numTrailingBackslashes;
}

std::string uppercase(const std::string & iValue)
{
  std::string out;
  for(size_t i=0; i<iValue.size(); i++)
  {
    char c = (char)toupper(iValue[i]);
    out.append(1, c);
  }
  return out;
}

std::string uppercase(const char * iValue)
{
  if (iValue == NULL)
    return "";
  return uppercase(std::string(iValue));
}

}; //namespace utils
}; //namespace libargvcodec
