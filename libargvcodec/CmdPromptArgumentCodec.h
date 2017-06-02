#pragma once

#include "IArgumentEncoder.h"
#include "IArgumentDecoder.h"

  class LIBARGVCODEC_API CmdPromptArgumentCodec : public virtual IArgumentEncoder,
                                            public virtual IArgumentDecoder
  {
  public:
    CmdPromptArgumentCodec();
    virtual ~CmdPromptArgumentCodec();

    //IArgumentEncoder
    virtual std::string encodeArgument(const char * iValue);
    virtual std::string encodeCommandLine(const ArgumentList & iArguments);

    //IArgumentDecoder
    virtual std::string decodeArgument(const char * iValue);
    virtual ArgumentList decodeCommandLine(const char * iValue);

  public:
    virtual bool isArgumentSeparator(const char c);
    virtual bool isShellCharacter(const char c);
    virtual bool hasShellCharacters(const char * iValue);
    virtual bool supportsShellCharacters();

  protected:
    bool parseCmdLine(const char * iCmdLine, ArgumentList::StringList & oArguments);
    char getSafeCharacter(const char * iValue, size_t iIndex);
    bool matchesSequence(const char * iValue, const char * iSequenceExpr);
    bool matchesSequence(const char * iValue, size_t iValueOffset, const char * iSequenceExpr);
    bool matchesBackSlashDblQuoteSequence(const char * iValue, size_t iValueOffset, size_t & oNumBlackSlash, size_t & oSkipLength, bool iInString, bool iInCaretString);
    bool isStringStart(const char * iCmdLine, size_t iOffset);
    bool isStringEnd(const char * iCmdLine, size_t iOffset, size_t iSequenceLength);

  };
