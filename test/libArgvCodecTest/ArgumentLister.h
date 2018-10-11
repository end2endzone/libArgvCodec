#pragma once

#include "libargvcodec/ArgumentList.h"

using namespace libargvcodec;

std::string getExePath();
bool isPrinterExec();
bool isLoggerExec();
std::string getPrinterExecFilePath();
std::string getLoggerExecFilePath();
std::string getLogFilePath();

int printArgs(int argc, char* argv[]);
int logArgs(int argc, char* argv[]);

/*
 Description:
  Copy itself as *.printer.exe and *.logger.exe to properly detect between the following behavior:
    1. Run all tests cases
    2. Print given arguments to the screen.
    3. Log given arguments to a fileexecuting test cases, printing arguments
*/
void initExecStubs();

std::string systemDecodeArgument(const char * iValue);
bool systemDecodeCommandLineArguments(const char * iCmdLine, ArgumentList::StringList & oArguments);
std::string createProcessDecodeArgument(const char * iValue);
bool createProcessDecodeCommandLineArguments(const char * iCmdLine, ArgumentList::StringList & oArguments);
