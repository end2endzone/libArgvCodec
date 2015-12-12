#include "CreateProcessArgumentCodec.h"

CreateProcessArgumentCodec::CreateProcessArgumentCodec()
{
}

CreateProcessArgumentCodec::~CreateProcessArgumentCodec()
{
}

////IArgumentEncoder
//std::string CreateProcessArgumentCodec::encodeArgument(const char * iValue)
//{
//}
//
//std::string CreateProcessArgumentCodec::encodeCommandLine(const ArgumentList & iArguments)
//{
//}
//
////IArgumentDecoder
//std::string CreateProcessArgumentCodec::decodeArgument(const char * iValue)
//{
//}
//
//ArgumentList CreateProcessArgumentCodec::decodeCommandLine(const char * iValue)
//{
//}

bool CreateProcessArgumentCodec::isShellCharacter(const char c)
{
  return false; //No such thing as shell characters
}

bool CreateProcessArgumentCodec::hasShellCharacters(const char * iValue)
{
  return false; //No such thing as shell characters
}

bool CreateProcessArgumentCodec::supportsShellCharacters()
{
  return false;
}
