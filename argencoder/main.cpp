// argencoder.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>

#include "ArgumentList.h"
#include "CmdPromptArgumentCodec.h"
#include "CreateProcessArgumentCodec.h"

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

	return 0;
}
