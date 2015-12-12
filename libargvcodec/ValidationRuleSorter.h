#pragma once

#include "advancedsort.h"
#include "ValidationRuleList.h"

class ValidationRuleSorter : public advancedsort::SortFunctionT<VALIDATION_RULE*>
{
public:
  typedef VALIDATION_RULE * VALIDATION_RULE_PTR;
  virtual ~ValidationRuleSorter();
  advancedsort::SortAttributeList getSortAttributes(const VALIDATION_RULE_PTR & iObject);

private:
  bool * newBool(bool iValue);
  std::vector<bool*> mBoolInstances;

};
