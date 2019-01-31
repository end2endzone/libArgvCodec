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

#include <stdio.h>
#include <iostream>

#include "libargvcodec/version.h"
#include "libargvcodec/ArgumentList.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "libargvcodec/CreateProcessArgumentCodec.h"
#include "libargvcodec/TerminalArgumentCodec.h"

#include "rapidassist/console.h"

using namespace libargvcodec;
using namespace std;

void printHeader()
{
  cout << "argencoder v" << LIBARGVCODEC_VERSION << " - command line encoder" << endl;
  cout << endl;
}

int main(int argc, char* argv[])
{
  printHeader();

  int numArguments = 0;
  cout << "Enter the number of arguments to encode: ";
  cin >> numArguments;
  cout << endl;

  //capture the remaining ENTER key
  std::string tmp;
  std::getline(std::cin, tmp);

  //validate user input
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

  //build the list of arguments
  ArgumentList arglist;
  arglist.insert("foo.exe"); //argv[0] is always the path of the current process

  cout << "Enter the value of each arguments: " << endl;
  for(int i=0; i<numArguments; i++)
  {
    cout << "#" << i+1 << ": ";

    std::string value;
    std::getline(std::cin, value);

    arglist.insert(value.c_str());
  }
  cout << "done." << endl;
  cout << endl;

  cout << "The result command line for each encoder is the following:" << endl;
  cout << endl;

  //Encode
  {
    CmdPromptArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "Windows Command Prompt:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }
  {
    CreateProcessArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "Windows CreateProcess() API:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }
  {
    TerminalArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "Linux Terminal:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }

  //is this application that spawned a console?
  //if so, then we ask the user to press a key before it goes away
  if (ra::console::hasConsoleOwnership())
  {
    cout << "Press a key to terminate application..." << endl;
    getchar();
  }

	return 0;
}
