#pragma once

#include "IArgumentEncoder.h"
#include "IArgumentDecoder.h"

class ARGMGR_API CmdPromptArgumentCodec : public virtual IArgumentEncoder, public virtual IArgumentDecoder
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
  static bool isArgumentSeparator(const char c);
  static bool isShellCharacter(const char c);
  static bool hasShellCharacters(const char * iValue);

protected:
  bool parseCmdLine(const char * iCmdLine, ArgumentList::StringList & oArguments);

};
