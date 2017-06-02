// showargs.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  for(int i=0; i<argc; i++)
  {
    const char * value = argv[i];
    cout << "arg" << i << ": " << value << endl;
  }
	return 0;
}
