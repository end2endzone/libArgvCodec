#pragma once

#include "ArgMgrExports.h"
#include "ArgumentList.h"

class ARGMGR_API IArgumentEncoder
{
public:
  //
  // Description:
  //  Encodes a single argument
  // Parameters:
  //  iValue:    The value of the argument
  //
  virtual std::string encodeArgument(const char * iValue) = 0;

  //
  // Description:
  //  Encodes an argument's list into a command line
  // Parameters:
  //  iArguments:    The list of arguments
  //
  virtual std::string encodeCommandLine(const ArgumentList & iArguments) = 0;
};