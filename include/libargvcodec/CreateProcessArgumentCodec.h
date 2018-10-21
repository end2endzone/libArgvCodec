#ifndef CREATEPROCESSARGUMENTCODEC_H
#define CREATEPROCESSARGUMENTCODEC_H

#include "IArgumentEncoder.h"
#include "IArgumentDecoder.h"
#include "CmdPromptArgumentCodec.h"

namespace libargvcodec
{

  class LIBARGVCODEC_EXPORT CreateProcessArgumentCodec : public CmdPromptArgumentCodec
  {
  public:
    CreateProcessArgumentCodec();
    virtual ~CreateProcessArgumentCodec();

    ////IArgumentEncoder interface
    //virtual std::string encodeArgument(const char * iValue);
    //virtual std::string encodeCommandLine(const ArgumentList & iArguments);

    ////IArgumentDecoder interface
    //virtual std::string decodeArgument(const char * iValue);
    //virtual ArgumentList decodeCommandLine(const char * iValue);

  public:
    virtual bool isShellCharacter(const char c);
    virtual bool hasShellCharacters(const char * iValue);
    virtual bool supportsShellCharacters();

  };

}; //namespace libargvcodec

#endif //CREATEPROCESSARGUMENTCODEC_H
