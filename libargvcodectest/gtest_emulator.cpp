#include "gtest_emulator.h"

bool registerTest(const char * iName, FunctionPointer f)
{
  Test t = {0};
  t.name = iName;
  t.f = f;

  if (gTests == NULL)
  {
    gTests = new TestList();
  }

  gTests->push_back(t);
  return true;
}

void RUN_ALL_TESTS()
{
  if (gTests == NULL)
    return;

  for(size_t i=0; i<gTests->size(); i++)
  {
    Test & t = (*gTests)[i];

    //detect if test is disabled
    bool testDisabled = false;
    static const std::string disabledToken = "DISABLED_";
    std::string testName = t.name;
    if (testName.find(disabledToken) != std::string::npos)
    {
      testDisabled = true;
    }

    //run test
    if (testDisabled)
    {
      printf("*** Warning: test \"%s\" is disabled.\n", t.name);
    }
    else
    {
      printf("Running test \"%s\"... ", t.name);
      t.f();
      printf("OK!\n");
    }
  }
}
