#pragma once

#include <vector>
#include <assert.h>

typedef void (*FunctionPointer)();

struct Test
{
  const char * name;
  bool success;
  FunctionPointer f;
};

typedef std::vector<Test> TestList;
static TestList * gTests = NULL;

bool registerTest(const char * iName, FunctionPointer f);
void RUN_ALL_TESTS();

//#define TEST_F(fixturename, testname) \
//namespace fixturename \
//{ \
//  void testname(); \
//  static bool testname##Registered = registerTest(#fixturename##testname, &::testname); \
//} \
//void fixturename::testname()

#define TEST_F(fixturename, testname) \
namespace fixturename \
{ \
  void testname(); \
  static bool testname##_register = registerTest(#fixturename##"."###testname, &fixturename::testname); \
}; \
void fixturename::testname()

#define ASSERT_EQ(val1, val2) assert(val1 == val2)
#define ASSERT_TRUE(expr) assert(expr)
#define ASSERT_FALSE(expr) assert(!(expr))
