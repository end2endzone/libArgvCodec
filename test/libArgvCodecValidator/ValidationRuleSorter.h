#pragma once

#include "AdvancedSort.h"
#include "ValidationRuleList.h"

namespace libargvcodec
{
namespace validation
{

  class ValidationRuleSorter : public advancedsort::SortFunctionT<ValidationRule*>
  {
  public:
    typedef ValidationRule * VALIDATION_RULE_PTR;
    virtual ~ValidationRuleSorter();
    advancedsort::SortAttributeList getSortAttributes(const VALIDATION_RULE_PTR & iObject);

    typedef std::vector<bool*> BoolPtrVector;

  private:
    bool * newBool(bool iValue);

    SAFE_WARNING_DISABLE(4251); //warning C4251: 'ArgumentList::mArguments' : // warning C4251: 'foo' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'bar'
    BoolPtrVector mBoolInstances;
    SAFE_WARNING_RESTORE();

  };

}; //namespace validation
}; //namespace libargvcodec
