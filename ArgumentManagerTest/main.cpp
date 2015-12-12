// ArgMgrTest.c pp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "ArgumentManager.h"
#include "gtest_emulator.h"

#include "ArgumentLister.h"

ArgumentManager gArgumentMgr;

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
  //Handle argument stubs
  if (isPrinterExec())
  {
    return printArgs(argc, argv);
  }
  if (isLoggerExec())
  {
    return logArgs(argc, argv);
  }
  initExecStubs();

  //Run tests
  RUN_ALL_TESTS();

  system("PAUSE");
	return 0;
}

