#pragma once

#include "libargvcodecvalidator.h"
#include "advancedsort.h"
#include "ValidationRuleList.h"

namespace libargvcodec
{

  class LIBARGVCODECVALIDATOR_API ValidationRuleSorter : public advancedsort::SortFunctionT<ValidationRule*>
  {
  public:
    typedef ValidationRule * VALIDATION_RULE_PTR;
    virtual ~ValidationRuleSorter();
    advancedsort::SortAttributeList getSortAttributes(const VALIDATION_RULE_PTR & iObject);

  private:
    bool * newBool(bool iValue);
    std::vector<bool*> mBoolInstances;

  };

}; //namespace libargvcodec
