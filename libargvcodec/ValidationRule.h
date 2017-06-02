#pragma once

#include "libargvcodec.h"
#include <string>

  class LIBARGVCODEC_API ValidationRule
  {
  public:
    struct FLAGS
    {
      bool isOptional         : 1;
      bool isMandatory        : 1;

      bool isFlag             : 1;
      bool hasValue           : 1;
      bool hasNextValue       : 1;

      bool caseSensitive      : 1;
      bool allowOtherPrefix   : 1;
      bool allowNullValues    : 1;
      //bool repeatable         : 1;
    };

    ValidationRule();
    ValidationRule(const ValidationRule & iValidationRule);
    ValidationRule(const char * iName, const FLAGS & iFlags);
    ValidationRule(const char * iName, const char * iDescription, const FLAGS & iFlags);
    virtual ~ValidationRule();

    void setInvalid();
    bool isValid() const;

    const std::string & getName() const;
    void setName(const char * iName);

    const std::string & getDescription() const;
    void setDescription(const char * iDescription);

    void setFlags(const FLAGS & iFlags);
    FLAGS getFlags() const;

    void set(const char * iName, const char * iDescription, const FLAGS & iFlags);

    //Basic flags
    class LIBARGVCODEC_API FlagFactory
    {
    public:
      static FLAGS optionalFlag();
      static FLAGS mandatoryFlag();
      static FLAGS optionalValue();
      static FLAGS mandatoryValue();
      static FLAGS optionalNextValue();
      static FLAGS mandatoryNextValue();
    };

    struct RESULT
    {
      bool validity;
      std::string errorDescription;
      std::string value;
    };
    //static const RESULT & getInvalidResult();

    const RESULT & getResult() const;
    void setResult(const RESULT & iResult);

  private:
    RESULT mResult;
    std::string mName;
    std::string mDescription;
    FLAGS mFlags;
  };
