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


// argencoder.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <conio.h>

#include "ArgumentList.h"
#include "CmdPromptArgumentCodec.h"
#include "CreateProcessArgumentCodec.h"
#include "libPromptConsole.h"

using namespace libargvcodec;
using namespace std;

void printHeader()
{
  cout << "argencoder v1.0 - command line encoder" << endl;
  cout << endl;
}

int main(int argc, _TCHAR* argv[])
{
  printHeader();

  int numArguments = 0;
  cout << "Enter the number of arguments to encode: ";
  cin >> numArguments;
  cout << endl;

  //capture the remaining ENTER key
  std::string tmp;
  std::getline(std::cin, tmp);

  //validate
  if (numArguments < 0)
  {
    cout << "invalid number of arguments" << endl;
    return 1;
  }
  else if (numArguments == 0)
  {
    //nothing to do
    return 0;
  }

  //build the list
  ArgumentList arglist;
  arglist.insert("foo.exe");

  for(int i=0; i<numArguments; i++)
  {
    cout << "Enter the value of argument #" << i+1 << ": ";

    std::string value;
    std::getline(std::cin, value);

    arglist.insert(value.c_str());
  }
  cout << "done." << endl;
  cout << endl;

  //Encode
  {
    CmdPromptArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "The encoded command line for the command prompt is the following:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }
  {
    CreateProcessArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "The encoded command line for CreatePaocess() api is the following:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }

  //is this application that spawned a console?
  //if so, then we ask the user to press a key before it goes away
  if (hasConsoleOwnership())
  {
    cout << "Press a key to terminate application..." << endl;
    _getch();
  }

	return 0;
}
