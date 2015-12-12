#pragma once

#include <vector>
#include <assert.h>

typedef void (*FunctionPointer)();

typedef std::vector<FunctionPointer> FunctionPointerList;
static FunctionPointerList gTestFunctions;

bool registerTest(FunctionPointer f);
void RUN_ALL_TESTS();

#define TEST_F(fixturename, testname) \
void testname(); \
static bool testname##Registered = registerTest(&::testname); \
void testname()

#define ASSERT_EQ(val1, val2) assert(val1 == val2)
#define ASSERT_TRUE(expr) assert(expr)
#define ASSERT_FALSE(expr) assert(!(expr))
