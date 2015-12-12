#pragma once

#include "libargvcodec.h"
#include "advancedsort.h"
#include "ValidationRuleList.h"

class LIBARGVCODEC_API ValidationRuleSorter : public advancedsort::SortFunctionT<ValidationRule*>
{
public:
  typedef ValidationRule * VALIDATION_RULE_PTR;
  virtual ~ValidationRuleSorter();
  advancedsort::SortAttributeList getSortAttributes(const VALIDATION_RULE_PTR & iObject);

private:
  bool * newBool(bool iValue);
  std::vector<bool*> mBoolInstances;

};
