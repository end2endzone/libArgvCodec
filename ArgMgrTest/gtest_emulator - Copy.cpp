#include "gtest_emulator.h"

bool registerTest(FunctionPointer f)
{
  gTestFunctions.push_back(f);
  return true;
}

void RUN_ALL_TESTS()
{
  for(size_t i=0; i<gTestFunctions.size(); i++)
  {
    FunctionPointer f = gTestFunctions[i];
    f();
  }
}
