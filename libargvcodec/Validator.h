#pragma once

#include "libargvcodec.h"
#include "ArgumentList.h"
#include "ValidationResultList.h"
#include "ValidationRuleList.h"
#include <string>
#include <vector>

class LIBARGVCODEC_API Validator
{
public:
  Validator();
  virtual ~Validator();

  ValidationResultList validate(const ArgumentList & iArgumentList, const ValidationRuleList & iRules);

  //global options
  void setOrphanArgumentsAccepted(bool iOrphanArgumentsAccepted);

  //static 
  static void sortRules(ValidationRuleList & ioRules);

private:
  //private attributes
  bool mOrphanArgumentsAccepted;
  ArgumentList::StringList mDefaultArgumentPrefixes;

  //private methods
  bool validateRule(const VALIDATION_RULE * iRule, std::string & oErrorMessage);
  VALIDATION_RESULT processRule(const VALIDATION_RULE * iRule, ArgumentList & ioArgumentList);
  void applyDefaultArgumentPrefixes(ArgumentList & ioArgumentList);

};
