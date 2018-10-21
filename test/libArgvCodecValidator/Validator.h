#pragma once

#include "libargvcodec/ArgumentList.h"
#include "ValidationRuleList.h"
#include <string>
#include <vector>

namespace libargvcodec
{
namespace validation
{

  class Validator
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

    SAFE_WARNING_DISABLE(4251); //warning C4251: 'ArgumentList::mArguments' : // warning C4251: 'foo' : class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'bar'
    ArgumentList::StringList mDefaultArgumentPrefixes;
    SAFE_WARNING_RESTORE();

    //private methods
    bool validateRule(const ValidationRule * iRule, std::string & oErrorMessage);
    void processRule(ValidationRule * iRule, ArgumentList & ioArgumentList);
    void applyDefaultArgumentPrefixes(ArgumentList & ioArgumentList);
    //ValidationRuleList makeCopy(ValidationRuleList & iRuleList);

  };

}; //namespace validation
}; //namespace libargvcodec
