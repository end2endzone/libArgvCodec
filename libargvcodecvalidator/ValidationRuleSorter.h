#pragma once

#include "libargvcodecvalidator.h"
#include "advancedsort.h"
#include "ValidationRuleList.h"

namespace libargvcodec
{
namespace validation
{

  class LIBARGVCODECVALIDATOR_API ValidationRuleSorter : public advancedsort::SortFunctionT<ValidationRule*>
  {
  public:
    typedef ValidationRule * VALIDATION_RULE_PTR;
    virtual ~ValidationRuleSorter();
    advancedsort::SortAttributeList getSortAttributes(const VALIDATION_RULE_PTR & iObject);

    typedef std::vector<bool*> BoolPtrVector;
    VALIDATOR_EXPORT_STL_VECTOR(LIBARGVCODECVALIDATOR_API, bool *);

  private:
    bool * newBool(bool iValue);
    #pragma warning(push)
    #pragma warning(disable: 4251)  // warning C4251: 'foo' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'bar'
    BoolPtrVector mBoolInstances;
    #pragma warning(pop)

  };

}; //namespace validation
}; //namespace libargvcodec
