// test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <stdio.h>

#include <gtest/gtest.h>
#include "rapidassist/gtesthelp.h"

#include "ArgumentLister.h"

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
  if (ra::gtesthelp::isProcessorX86())
  {
    if (ra::gtesthelp::isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x86.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x86.release.xml";
  }
  else if (ra::gtesthelp::isProcessorX64())
  {
    if (ra::gtesthelp::isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x64.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:libargvcodectest.x64.release.xml";
  }

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  //Disable Windows tests on Linux
#ifdef __linux__
  ::testing::GTEST_FLAG(filter) = ra::gtesthelp::mergeFilter("", "TestCmdPromptArgumentCodec.*:TestCreateProcessArgumentCodec.*", ::testing::GTEST_FLAG(filter).c_str());
#endif

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
