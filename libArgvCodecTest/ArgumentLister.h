//
//  libArgvCodec Library - v1.0 - 06/25/2017
//  Copyright (C) 2017 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  
//  The libArgvCodec library is used when an application has to launch another one.
//  It includes arguments and command line encoder/decoder (codecs) that are
//  designed for the purpose of helping a developper in this situation. The
//  libArgvCodec library is designed to avoid typical encoding and decoding pitfalls
//  when transferring local variable values to other application through command
//  line arguments.
//
// HISTORY:
// 06/25/2017 v1.0 - Initial release.
//


#pragma once

#include "ArgumentList.h"

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
