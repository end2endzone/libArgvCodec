#ifndef TESTARGUMENTLIST_H
#define TESTARGUMENTLIST_H

#include <gtest/gtest.h>

class TestArgumentList : public ::testing::Test
{
public:
  virtual void SetUp();
  virtual void TearDown();
};

#endif //TESTARGUMENTLIST_H
