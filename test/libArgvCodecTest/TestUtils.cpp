#include "TestUtils.h"
#include <cstdio> //for sprintf()
#include <string.h> //for strdup()
#include <cstdlib> //for free()

DynamicStringList gStrings;

//register a new string for the current test
char * mkstr(const char * value)
{
#ifdef _WIN32
  char * _copy = _strdup(value);
#else
  char * _copy = strdup(value);
#endif
  gStrings.push_back(_copy);
  return _copy;
}

void clearDynamicStrings()
{
  //clear all strings created in the test
  for(size_t i=0; i<gStrings.size(); i++)
  {
    char * value = gStrings[i];
    free(value);
  }
  gStrings.clear();
}

std::string getSequencedFile(const char * iPrefix, int iValue, const char * iPostfix, int iValueLength)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];

  char valueFormat[BUFFER_SIZE];
  char value[BUFFER_SIZE];
  sprintf(valueFormat, "%%0%dd", iValueLength);
  sprintf(value, valueFormat, iValue);

  sprintf(buffer, "%s%s%s", iPrefix, value, iPostfix);

  return std::string(buffer);
}
