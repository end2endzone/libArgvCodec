// ArgMgrTest.c pp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "ArgumentList.h"

#include <gtest/gtest.h>
#include "gtesthelper.h"

#include "ArgumentLister.h"

//libargvcodec::ArgumentList gArgumentMgr;

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

  //init google test
  gTestHelper & hlp = gTestHelper::getInstance();
  if (hlp.isProcessorX86())
  {
    if (hlp.isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x86.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x86.release.xml";
  }
  else if (hlp.isProcessorX64())
  {
    if (hlp.isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x64.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x64.release.xml";
  }

  //ArgumentList::StringList args;
  //bool a = createProcessDecodeCommandLineArguments("test \"allo toi\" \"\" ^\"test^\" coucou", args);
  //std::string b = createProcessDecodeArgument("ab");
  //int c = 0;

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
