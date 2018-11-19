#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>
#include <vector>

#include "libargvcodec/ArgumentList.h"

#include "rapidassist/strings.h"

//string memory management
typedef std::vector<char*> DynamicStringList;
char * mkstr(const char * value);
void clearDynamicStrings();
bool isDashedLine(const std::string & iLine);

std::string getSequencedFile(const char * iPrefix, int iValue, const char * iPostfix, int iValueLength);

std::string getShowArgsExecutablePath();
bool getArgumentsFromSystem(const std::string & iCmdline, ra::strings::StringVector & oArguments);
bool getArgumentsFromCreateProcess(const std::string & iCmdline, ra::strings::StringVector & oArguments);

ra::strings::StringVector toStringList(const libargvcodec::ArgumentList & arguments);

//error string builders for ASSERT macro.
std::string buildErrorString(const std::string & iVar1, const ra::strings::StringVector & iList1, const std::string & iVar2, const ra::strings::StringVector & iList2);
std::string buildErrorString(const std::string & iCmdLine, const ra::strings::StringVector & iExpectedArguments, const ra::strings::StringVector & iActualArguments);

#endif //TESTUTILS_H
