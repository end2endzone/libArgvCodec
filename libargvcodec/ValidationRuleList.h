#pragma once

#include "libargvcodec.h"
#include "ValidationRule.h"
#include <vector>

class LIBARGVCODEC_API ValidationRuleList
{
public:
  ValidationRuleList();
  ValidationRuleList(const ValidationRuleList & iValidationRuleList);
  virtual ~ValidationRuleList();

  typedef std::vector<ValidationRule*> ValidationRulePtrList;

  const ValidationRulePtrList & getRules() const;
  ValidationRulePtrList & getRules();

  //
  // Description:
  //  Adds a new rule to the list. The list takes ownership of the rule.
  // Parameters:
  //  iRule: The new rule to add.
  //
  void addRule(ValidationRule* iRule);

  //
  // Description:
  //  Clears the list of rules
  //
  void clear();

  //rule modification methods
  void setAllRulesInvalid();
  
  //rules query methods
  bool isAllRulesValid();
  const ValidationRule * getFailedRuleAbout(const char * iArgumentName);
  const ValidationRule * getValidRuleAbout(const char * iArgumentName);
  bool hasFailedRuleAbout(const char * iArgumentName);
  bool hasValidRuleAbout(const char * iArgumentName);

private:
  //methods
  void copyFrom(const ValidationRuleList & ioRuleList);

  //attributes

  //Disable the following warning: 
  //    warning C4251: 'ValidationRuleList::mRules' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'ValidationRuleList'
#ifdef _MSC_VER
 #pragma warning( push )
 #pragma warning( disable: 4251 )
#endif
  ValidationRulePtrList mRules;
#ifdef _MSC_VER
 #pragma warning( pop )
#endif
};
