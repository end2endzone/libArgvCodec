#include "utils.h"

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

bool readTextFile(const char * iPath, StringList & oLines)
{
  oLines.clear();

  FILE * f = fopen(iPath, "r");
  if ( f == NULL )
    return false;

  static const int BUFFER_SIZE = 10240;
  char buffer[BUFFER_SIZE];

  while( fgets(buffer, BUFFER_SIZE, f) != NULL )
  {
    std::string line = buffer;
    strReplace(line, "\n", "");

    oLines.push_back(line);
  }

  fclose(f);

  return true;
}

bool fileExists(const char * iPath)
{
  FILE * f = fopen(iPath, "rb");
  if (f == NULL)
    return false;
  fclose(f);
  return true;
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

}; //namespace utils