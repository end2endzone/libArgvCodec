#pragma once

#include "ArgMgrExports.h"
#include "ArgumentList.h"

class ARGMGR_API IArgumentEncoder
{
public:
  virtual std::string encodeArgument(const char * iValue) = 0;
  virtual std::string encodeCommandLine(const ArgumentList & iArguments) = 0;
};
