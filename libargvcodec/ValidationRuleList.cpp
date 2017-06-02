#include "ValidationRuleList.h"

namespace libargvcodec
{

ValidationRuleList::ValidationRuleList()
{
}

ValidationRuleList::ValidationRuleList(const ValidationRuleList & iValidationRuleList)
{
  copyFrom(iValidationRuleList);
}

ValidationRuleList::~ValidationRuleList()
{
  clear();
}

const ValidationRuleList::ValidationRulePtrList & ValidationRuleList::getRules() const
{
  return mRules;
}

ValidationRuleList::ValidationRulePtrList & ValidationRuleList::getRules()
{
  return mRules;
}

void ValidationRuleList::addRule(ValidationRule* iRule)
{
  mRules.push_back(iRule);
}

void ValidationRuleList::clear()
{
  //delete all rules
  for(size_t i=0; i<mRules.size(); i++)
  {
    ValidationRule * rule = mRules[i];
    if (rule)
    {
      delete rule;
    }
  }

  //clear the internal list
  mRules.clear();
}

void ValidationRuleList::setAllRulesInvalid()
{
  for(size_t i=0; i<mRules.size(); i++)
  {
    ValidationRule * rule = mRules[i];
    if (rule)
    {
      rule->setInvalid();
    }
  }
}

bool ValidationRuleList::isAllRulesValid()
{
  for(size_t i=0; i<mRules.size(); i++)
  {
    ValidationRule * rule = mRules[i];
    if (rule)
    {
      if (!rule->isValid())
      {
        return false;
      }
    }
  }
  return true;
}

const ValidationRule * ValidationRuleList::getFailedRuleAbout(const char * iArgumentName)
{
  for(size_t i=0; i<mRules.size(); i++)
  {
    const ValidationRule * rule = mRules[i];
    const ValidationRule::RESULT & result = rule->getResult();
    if (!result.valid)
    {
      size_t pos = rule->getResult().errorDescription.find(iArgumentName);
      if (pos != std::string::npos)
      {
        return rule;
      }
    }
  }
  return NULL;
}

const ValidationRule * ValidationRuleList::getValidRuleAbout(const char * iArgumentName)
{
  for(size_t i=0; i<mRules.size(); i++)
  {
    const ValidationRule * rule = mRules[i];
    const ValidationRule::RESULT & result = rule->getResult();
    if (result.valid == true)
    {
      size_t pos = rule->getName().find(iArgumentName);
      if (pos != std::string::npos)
      {
        return rule;
      }
    }
  }
  return NULL;
}

bool ValidationRuleList::hasFailedRuleAbout(const char * iArgumentName)
{
  return getFailedRuleAbout(iArgumentName) != NULL;
}

bool ValidationRuleList::hasValidRuleAbout(const char * iArgumentName)
{
  return getValidRuleAbout(iArgumentName) != NULL;
}

void ValidationRuleList::copyFrom(const ValidationRuleList & ioRuleList)
{
  //Note: since the list takes ownership of the rules. A deep copy must be executed to copy the content of the given list

  for(size_t i=0; i<ioRuleList.getRules().size(); i++)
  {
    const ValidationRule * rule = ioRuleList.getRules()[i];
    if (rule)
    {
      ValidationRule * copy = new ValidationRule(*rule);
      if (copy)
      {
        addRule( copy );
      }
    }
  }
}

}; //namespace libargvcodec
