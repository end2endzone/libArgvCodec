// ArgMgrTest.c pp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "ArgumentList.h"
#include "gtest_emulator.h"

#include "ArgumentLister.h"

ArgumentList gArgumentMgr;

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

  ArgumentList::StringList args;
  bool a = createProcessDecodeCommandLineArguments("test \"allo toi\" \"\" ^\"test^\" coucou", args);
  std::string b = createProcessDecodeArgument("ab");
  int c = 0;

  //Run tests
  RUN_ALL_TESTS();

  system("PAUSE");
	return 0;
}
