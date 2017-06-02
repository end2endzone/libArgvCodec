#pragma once

#include "libargvcodecvalidator.h"
#include "ArgumentList.h"
#include "ValidationRuleList.h"
#include <string>
#include <vector>

namespace libargvcodec
{

  class LIBARGVCODECVALIDATOR_API Validator
  {
  public:
    Validator();
    virtual ~Validator();

    bool validate(const ArgumentList & iArgumentList, ValidationRuleList & ioRuleList);

    //global options
    void setUnknownArgumentAsError(bool iOrphanArgumentsAccepted);

    //static 
    static void sortRules(ValidationRuleList & ioRuleList);

  private:
    //private attributes
    bool mUnknownArgumentAsError;
    ArgumentList::StringList mDefaultArgumentPrefixes;

    //private methods
    bool validateRule(const ValidationRule * iRule, std::string & oErrorMessage);
    void processRule(ValidationRule * iRule, ArgumentList & ioArgumentList);
    void applyDefaultArgumentPrefixes(ArgumentList & ioArgumentList);
    //ValidationRuleList makeCopy(ValidationRuleList & iRuleList);

  };

}; //namespace libargvcodec
