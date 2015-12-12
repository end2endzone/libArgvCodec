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
    printf("Running test \"%s\"... ", t.name);
    t.f();
    printf("OK!\n");
  }
}
