#include "targetver.h"

#include "Validator.h"
#include "ValidationRuleSorter.h"
#include "utils.h"
#include "os.h"
#include <algorithm>

namespace libargvcodec
{
namespace validation
{

Validator::Validator()
: mUnknownArgumentAsError(false)
{
  //build default list of prefixes
  mDefaultArgumentPrefixes.clear();
  mDefaultArgumentPrefixes.push_back("");
  mDefaultArgumentPrefixes.push_back("/");
  mDefaultArgumentPrefixes.push_back("-");
  mDefaultArgumentPrefixes.push_back("--");
}

Validator::~Validator()
{
}

bool Validator::validate(const ArgumentList & iArgumentList, ValidationRuleList & ioRuleList)
{
  bool validationPass = false;

  ArgumentList arguments = iArgumentList; //copy arguments since we are going to remove each argument from the list to detect missing or unknown arguments

  //remove the exe name from the argument's list
  if (arguments.getArgc() > 0)
  {
    arguments.remove(0);
  }

  //sort
  sortRules(ioRuleList);

  ValidationRuleList::ValidationRulePtrList & rules = ioRuleList.getRules();

  //process each rule
  for(size_t i=0; i<rules.size(); i++)
  {
    ValidationRule * rule = rules[i];
    processRule(rule, arguments);
  }

  //check for orphan argument
  if (!mUnknownArgumentAsError && arguments.getArgc() > 0)
  {
    //define failed rules for each remaining arguments
    static const int BUFFER_SIZE = 1024;
    char errorMessage[BUFFER_SIZE];

    for(int i=0; i<arguments.getArgc(); i++)
    {
      const char * argValue = arguments.getArgument(i);

      //define failed rule for argument
      ValidationRule * rule = new ValidationRule(argValue, "", ValidationRule::FlagFactory::mandatoryFlag());

      sprintf(errorMessage, "Unknown argument \"%s\".", argValue);

      ValidationRule::RESULT r;
      r.valid = false;
      r.errorDescription = errorMessage;

      rule->setResult(r);

      ioRuleList.addRule(rule);
    }
  }

  return ioRuleList.isAllRulesValid();
}

void Validator::setUnknownArgumentAsError(bool iOrphanArgumentsAccepted)
{
  this->mUnknownArgumentAsError = iOrphanArgumentsAccepted;
}

void Validator::sortRules(ValidationRuleList & ioRuleList)
{
  ValidationRuleList::ValidationRulePtrList & rules = ioRuleList.getRules();
  
  std::sort(rules.begin(), rules.end(), ValidationRuleSorter());
}

bool Validator::validateRule(const ValidationRule * iRule, std::string & oErrorMessage)
{
  oErrorMessage = "";

  if (iRule == NULL)
  {
    oErrorMessage = "Rule is NULLArgument must be mandatory or optionnal";
    return false;
  }

  //validate rule content
  const ValidationRule::FLAGS & flags = iRule->getFlags();

  //mandatory or optionnal but not both
  bool mandatoryValid = (flags.isMandatory ^ flags.isOptional);
  if (!mandatoryValid)
  {
    oErrorMessage = "Argument must be mandatory or optionnal";
    return false;
  }

  //value validation
  bool valueValid = (( flags.isFlag ^ flags.hasValue) ^ flags.hasNextValue);
  if (!valueValid)
  {
    oErrorMessage = "Argument must be defined as a flag (switch), value or must be preceding a value";
    return false;
  }

  return true;
}

void Validator::processRule(ValidationRule * iRule, ArgumentList & ioArgumentList)
{
  iRule->setInvalid();
  ValidationRule::RESULT r;
  r.valid = false;

  //validate rule
  if (!validateRule(iRule, r.errorDescription))
  {
    iRule->setResult(r);
    return; //failed
  }
  r.errorDescription = "";

  const ValidationRule::FLAGS & flags = iRule->getFlags();
  const char * name = iRule->getName().c_str();

  //define allowed prefixes when searching into the ArgumentList
  if (flags.allowOtherPrefix)
  {
    applyDefaultArgumentPrefixes(ioArgumentList);
  }
  else
  {
    ioArgumentList.clearOptionPrefixes();
  }

  static const int BUFFER_SIZE = 1024;
  char errorMessage[BUFFER_SIZE];

  //check each flag
  if (flags.isMandatory)
  {
    //mandatory arguments
    if (flags.isFlag)
    {
      bool optionFound = ioArgumentList.extractOption(name, flags.caseSensitive);
      if (!optionFound)
      {
        sprintf(errorMessage, "Mandatory argument \"%s\" is not found.", name);
        r.errorDescription = errorMessage;
        iRule->setResult(r);
        return; //failed
      }
      r.value = name; //set the rule's value to the argument name to prevent null/empty
    }
    else if (flags.hasValue)
    {
      bool success = ioArgumentList.extractValue(name, flags.caseSensitive, r.value);
      if (!success)
      {
        sprintf(errorMessage, "Mandatory argument \"%s\" is not found or does not have a value.", name);
        r.errorDescription = errorMessage;
        iRule->setResult(r);
        return; //failed
      }
    }
    else if (flags.hasNextValue)
    {
      bool success = ioArgumentList.extractNextValue(name, flags.caseSensitive, r.value);
      if (!success)
      {
        sprintf(errorMessage, "Mandatory argument \"%s\" is not found or is not followed by a value.", name);
        r.errorDescription = errorMessage;
        iRule->setResult(r);
        return; //failed
      }
    }
  }
  else
  {
    //optional arguments
    if (flags.isFlag)
    {
      bool optionFound = ioArgumentList.extractOption(name, flags.caseSensitive);
      if (optionFound)
      {
        r.value = name; //set the rule's value to the argument name to prevent null/empty
      }
    }
    else if (flags.hasValue)
    {
      bool success = ioArgumentList.extractValue(name, flags.caseSensitive, r.value);
    }
    else if (flags.hasNextValue)
    {
      bool success = ioArgumentList.extractNextValue(name, flags.caseSensitive, r.value);
    }
  }

  if (!flags.isFlag && !flags.allowNullValues && r.value == "")
  {
    sprintf(errorMessage, "Argument \"%s\" is not allowed to be null/empty.", name);
    r.errorDescription = errorMessage;
    iRule->setResult(r);
    return; //failed
  }

  //expected successful result at this point
  r.valid = true;
  iRule->setResult(r);
  return; //success
}

void Validator::applyDefaultArgumentPrefixes(ArgumentList & ioArgumentList)
{
  ioArgumentList.clearOptionPrefixes();
  for(size_t i=0; i<mDefaultArgumentPrefixes.size(); i++)
  {
    const std::string & prefix = mDefaultArgumentPrefixes[i];
    ioArgumentList.addOptionPrefix(prefix.c_str());
  }
}

//ValidationRuleList Validator::makeCopy(ValidationRuleList & iRuleList)
//{
//  ValidationRuleList newList;
//
//  ValidationRuleList::ValidationRulePtrList & rules = iRuleList.getRules();
//  for(size_t i=0; i<rules.size(); i++)
//  {
//    newList.addRule( rules[i] );
//  }
//
//  return newList;
//}

}; //namespace validation
}; //namespace libargvcodec
