#include "ValidationRule.h"

namespace libargvcodec
{
namespace validation
{

ValidationRule::ValidationRule()
{
  memset(&mFlags, 0, sizeof(mFlags));
  setInvalid();
}

ValidationRule::ValidationRule(const ValidationRule & iValidationRule)
: mResult( iValidationRule.mResult)
, mName( iValidationRule.mName )
, mDescription( iValidationRule.mDescription )
, mFlags( iValidationRule.mFlags )
{
  setInvalid();
}

ValidationRule::ValidationRule(const char * iName, const FLAGS & iFlags)
{
  memset(&mFlags, 0, sizeof(mFlags));

  set(iName, "", iFlags);
}

ValidationRule::ValidationRule(const char * iName, const char * iDescription, const FLAGS & iFlags)
{
  memset(&mFlags, 0, sizeof(mFlags));

  set(iName, iDescription, iFlags);
}

ValidationRule::~ValidationRule()
{
}

void ValidationRule::setInvalid()
{
  mResult.valid = false;
}

bool ValidationRule::isValid() const
{
  return mResult.valid;
}

const std::string & ValidationRule::getName() const
{
  return mName;
}

void ValidationRule::setName(const char * iName)
{
  mName = iName;
}

const std::string & ValidationRule::getDescription() const
{
  return mDescription;
}

void ValidationRule::setDescription(const char * iDescription)
{
  mDescription = iDescription;
}

void ValidationRule::setFlags(const ValidationRule::FLAGS & iFlags)
{
  mFlags = iFlags;
}

ValidationRule::FLAGS ValidationRule::getFlags() const
{
  return mFlags;
}

void ValidationRule::set(const char * iName, const char * iDescription, const FLAGS & iFlags)
{
  setName(iName);
  setDescription(iDescription);
  setFlags(iFlags);
}

ValidationRule::FLAGS ValidationRule::FlagFactory::optionalFlag()
{
  ValidationRule::FLAGS f;
  memset(&f, 0, sizeof(f));

  f.isOptional = true;
  f.isFlag = true;

  return f;
}

ValidationRule::FLAGS ValidationRule::FlagFactory::mandatoryFlag()
{
  ValidationRule::FLAGS f;
  memset(&f, 0, sizeof(f));

  f.isMandatory = true;
  f.isFlag = true;

  return f;
}

ValidationRule::FLAGS ValidationRule::FlagFactory::optionalValue()
{
  ValidationRule::FLAGS f;
  memset(&f, 0, sizeof(f));

  f.isOptional = true;
  f.hasValue = true;

  return f;
}

ValidationRule::FLAGS ValidationRule::FlagFactory::mandatoryValue()
{
  ValidationRule::FLAGS f;
  memset(&f, 0, sizeof(f));

  f.isMandatory = true;
  f.hasValue = true;

  return f;
}

ValidationRule::FLAGS ValidationRule::FlagFactory::optionalNextValue()
{
  ValidationRule::FLAGS f;
  memset(&f, 0, sizeof(f));

  f.isOptional = true;
  f.hasNextValue = true;

  return f;
}

ValidationRule::FLAGS ValidationRule::FlagFactory::mandatoryNextValue()
{
  ValidationRule::FLAGS f;
  memset(&f, 0, sizeof(f));

  f.isMandatory = true;
  f.hasNextValue = true;

  return f;
}

//const ValidationRule::RESULT & ValidationRule::getInvalidResult()
//{
//  static ValidationRule::RESULT r;
//  r.validity = false;
//  return r;
//}

const ValidationRule::RESULT & ValidationRule::getResult() const
{
  return mResult;
}

void ValidationRule::setResult(const ValidationRule::RESULT & iResult)
{
  mResult = iResult;
}

}; //namespace validation
}; //namespace libargvcodec
