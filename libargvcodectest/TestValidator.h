#ifndef TESTVALIDATOR_H
#define TESTVALIDATOR_H

#include <gtest/gtest.h>

class TestValidator : public ::testing::Test
{
public:
  virtual void SetUp();
  virtual void TearDown();
};

#endif //TESTVALIDATOR_H
