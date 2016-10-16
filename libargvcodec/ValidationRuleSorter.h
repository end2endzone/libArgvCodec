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

  //Disable the following warning: 
  //    warning C4251: 'ValidationRuleSorter::mBoolInstances' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'ValidationRuleSorter'
#ifdef _MSC_VER
 #pragma warning( push )
 #pragma warning( disable: 4251 )
#endif
  std::vector<bool*> mBoolInstances;
#ifdef _MSC_VER
 #pragma warning( pop )
#endif

};
