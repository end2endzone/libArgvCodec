#include "TestUtils.h"
#include <cstdio> //for sprintf()

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
