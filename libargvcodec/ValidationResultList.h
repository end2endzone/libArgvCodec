#pragma once

#include "libargvcodec.h"
#include "ValidationRuleList.h"
#include <string>
#include <vector>

struct VALIDATION_RESULT
{
  bool isValid;
  std::string description;
  std::string value;
  const VALIDATION_RULE * rule;
};

typedef std::vector<VALIDATION_RESULT> ValidationResultList;

LIBARGVCODEC_API bool isValid(const ValidationResultList & iResults);
