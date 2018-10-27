// argencoder.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>

#include "libargvcodec/version.h"
#include "libargvcodec/ArgumentList.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "libargvcodec/CreateProcessArgumentCodec.h"

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
  arglist.insert("foo.exe"); //argv[0] is always the path to the current process

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
    cout << "The encoded command line for the Command Prompt is the following:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }
  {
    CreateProcessArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "The encoded command line for CreateProcess() api is the following:" << endl;
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
