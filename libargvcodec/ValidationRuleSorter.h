#pragma once

#include "libargvcodec.h"
#include "macros.h"
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

    //warning C4251: 'ValidationRuleSorter::mBoolInstances' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'ValidationRuleSorter'
    SAFE_WARNING_DISABLE(4251);
    std::vector<bool*> mBoolInstances;
    SAFE_WARNING_RESTORE();

  };
