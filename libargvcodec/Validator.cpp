#include "targetver.h"

#include "Validator.h"
#include "ValidationRuleSorter.h"
#include "utils.h"
#include "os.h"
#include <algorithm>

Validator::Validator()
: mOrphanArgumentsAccepted(false)
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

ValidationResultList Validator::validate(const ArgumentList & iArgumentList, const ValidationRuleList & iRules)
{
  ValidationResultList results;

  ArgumentList arguments = iArgumentList; //copy arguments since we are going to remove each argument from the list to detect missing or unknown arguments
  ValidationRuleList rules = iRules; //copy rules since we are going to sort them by priority

  //remove the exe name from the argument's list
  if (arguments.getArgc() > 0)
  {
    arguments.remove(0);
  }

  //sort
  sortRules(rules);

  //for each rule
  for(size_t i=0; i<rules.size(); i++)
  {
    const VALIDATION_RULE * rule = rules[i];
    VALIDATION_RESULT result = processRule(rule, arguments);
    results.push_back( result );
  }

  //check for orphan argument
  if (!mOrphanArgumentsAccepted && arguments.getArgc() > 0)
  {
    //define failed rules for each remaining arguments
    static const int BUFFER_SIZE = 1024;
    char errorMessage[BUFFER_SIZE];

    for(int i=0; i<arguments.getArgc(); i++)
    {
      const char * argValue = arguments.getArgument(i);

      //define failed rule for argument
      VALIDATION_RESULT result;
      result.isValid = false;
      result.rule = NULL;

      sprintf(errorMessage, "Unknown argument \"%s\".", argValue);
      result.description = errorMessage;
    
      results.push_back(result);
    }
  }

  return results;
}

void Validator::setOrphanArgumentsAccepted(bool iOrphanArgumentsAccepted)
{
  this->mOrphanArgumentsAccepted = iOrphanArgumentsAccepted;
}

void Validator::sortRules(ValidationRuleList & ioRules)
{
  std::sort(ioRules.begin(), ioRules.end(), ValidationRuleSorter());
}

bool Validator::validateRule(const VALIDATION_RULE * iRule, std::string & oErrorMessage)
{
  oErrorMessage = "";

  if (iRule == NULL)
  {
    oErrorMessage = "Rule is NULLArgument must be mandatory or optionnal";
    return false;
  }

  //validate rule content
  
  //mandatory or optionnal but not both
  bool mandatoryValid = (iRule->flags.isMandatory ^ iRule->flags.isOptional);
  if (!mandatoryValid)
  {
    oErrorMessage = "Argument must be mandatory or optionnal";
    return false;
  }

  //value validation
  bool valueValid = (( iRule->flags.isFlag ^ iRule->flags.hasValue) ^ iRule->flags.hasNextValue);
  if (!valueValid)
  {
    oErrorMessage = "Argument must be defined as a flag (switch), value or must be preceding a value";
    return false;
  }

  return true;
}

VALIDATION_RESULT Validator::processRule(const VALIDATION_RULE * iRule, ArgumentList & ioArgumentList)
{
  VALIDATION_RESULT result;
  result.isValid = false;
  result.rule = iRule;

  //validate rule
  if (!validateRule(iRule, result.description))
  {
    return result;
  }
  result.description = "";

  //define allowed prefixes when searching into the ArgumentList
  if (iRule->flags.allowOtherPrefix)
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
  if (iRule->flags.isMandatory)
  {
    //mandatory arguments
    if (iRule->flags.isFlag)
    {
      bool optionFound = ioArgumentList.extractOption(iRule->argumentName.c_str(), iRule->flags.caseSensitive);
      if (!optionFound)
      {
        sprintf(errorMessage, "Mandatory argument \"%s\" is not found.", iRule->argumentName.c_str());
        result.description = errorMessage;
        return result;
      }
      result.value = iRule->argumentName.c_str(); //set the rule's value to the argument name to prevent null/empty
    }
    else if (iRule->flags.hasValue)
    {
      bool success = ioArgumentList.extractValue(iRule->argumentName.c_str(), iRule->flags.caseSensitive, result.value);
      if (!success)
      {
        sprintf(errorMessage, "Mandatory argument \"%s\" is not found or does not have a value.", iRule->argumentName.c_str());
        result.description = errorMessage;
        return result;
      }
    }
    else if (iRule->flags.hasNextValue)
    {
      bool success = ioArgumentList.extractNextValue(iRule->argumentName.c_str(), iRule->flags.caseSensitive, result.value);
      if (!success)
      {
        sprintf(errorMessage, "Mandatory argument \"%s\" is not found or is not followed by a value.", iRule->argumentName.c_str());
        result.description = errorMessage;
        return result;
      }
    }
  }
  else
  {
    //optional arguments
    if (iRule->flags.isFlag)
    {
      bool optionFound = ioArgumentList.extractOption(iRule->argumentName.c_str(), iRule->flags.caseSensitive);
      if (optionFound)
      {
        result.value = iRule->argumentName.c_str(); //set the rule's value to the argument name to prevent null/empty
      }
    }
    else if (iRule->flags.hasValue)
    {
      bool success = ioArgumentList.extractValue(iRule->argumentName.c_str(), iRule->flags.caseSensitive, result.value);
    }
    else if (iRule->flags.hasNextValue)
    {
      bool success = ioArgumentList.extractNextValue(iRule->argumentName.c_str(), iRule->flags.caseSensitive, result.value);
    }
  }

  if (!iRule->flags.isFlag && !iRule->flags.allowNullValues && result.value == "")
  {
    sprintf(errorMessage, "Argument \"%s\" is not allowed to be null/empty.", iRule->argumentName.c_str());
    result.description = errorMessage;
    return result;
  }

  //expected successful result at this point
  result.isValid = true;
  return result;
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
