#pragma once

#include "ArgMgrExports.h"
#include "ArgumentList.h"

class ARGMGR_API IArgumentDecoder
{
public:
  virtual std::string decodeArgument(const char * iValue) = 0;
  virtual ArgumentList decodeCommandLine(const char * iValue) = 0;
};
