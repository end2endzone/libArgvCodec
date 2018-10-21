// showargs.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>

#include "rapidassist/console.h"

using namespace std;

int main(int argc, char* argv[])
{
  for(int i=0; i<argc; i++)
  {
    const char * value = argv[i];
    cout << "arg" << i << ": " << value << endl;
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
