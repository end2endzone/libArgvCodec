#include "CreateProcessArgumentCodec.h"

CreateProcessArgumentCodec::CreateProcessArgumentCodec()
{
}

CreateProcessArgumentCodec::~CreateProcessArgumentCodec()
{
}

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
