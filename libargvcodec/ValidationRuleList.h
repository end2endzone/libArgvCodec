#pragma once

#include "libargvcodec.h"
#include "ArgumentList.h"
#include <string>
#include <vector>

#pragma pack(push, 1) // exact fit - no padding
struct ARGUMENT_FLAGS
{
  bool isOptional         : 1;
  bool isMandatory        : 1;

  bool isFlag             : 1;
  bool hasValue           : 1;
  bool hasNextValue       : 1;

  bool caseSensitive      : 1;
  bool allowOtherPrefix   : 1;
  bool allowNullValues    : 1;

  ARGUMENT_FLAGS()
  {
    isOptional         = false;
    isMandatory        = false;

    isFlag             = false;
    hasValue           = false;
    hasNextValue       = false;

    caseSensitive      = false;
    allowOtherPrefix   = false;
    allowNullValues    = false;
  }
};
#pragma pack(pop)
struct VALIDATION_RULE
{
  std::string argumentName;
  ARGUMENT_FLAGS flags;
};

typedef std::vector<VALIDATION_RULE*> ValidationRuleList;
