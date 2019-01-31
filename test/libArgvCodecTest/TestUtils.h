/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

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
std::string buildErrorString(const ra::strings::StringVector & iArguments, const std::string & iActualCmdLine, const ra::strings::StringVector & iActualArguments);

struct TEST_DATA
{
  std::string cmdline;
  ra::strings::StringVector arguments;
};
typedef std::vector<TEST_DATA> TEST_DATA_LIST;
bool loadCommandLineTestFile(const std::string & iPath, TEST_DATA_LIST & oItems);

#endif //TESTUTILS_H
