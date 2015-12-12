// ArgMgrTest.c pp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "ArgumentManager.h"
#include "gtest_emulator.h"

ArgumentManager gArgumentMgr;

std::string getExePath()
{
  ArgumentManager tmp;
  tmp.init("");
  const char * arg0 = tmp.getArgument(0);
  return std::string(arg0);
}

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
  RUN_ALL_TESTS();

  system("PAUSE");
	return 0;
}

