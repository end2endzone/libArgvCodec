#pragma once

#include "libargvcodec/config.h"
#include "ArgumentList.h"

namespace libargvcodec
{

  class LIBARGVCODEC_EXPORT IArgumentDecoder
  {
  public:

    //
    // Description:
    //  Decodes a single argument. Removes all encoding characters
    // Parameters:
    //  iValue:    The encoded argument
    //
    virtual std::string decodeArgument(const char * iValue) = 0;

    //
    // Description:
    //  Decodes a command line into a list of arguments
    // Parameters:
    //  iValue:    The value of the command line
    //
    virtual ArgumentList decodeCommandLine(const char * iValue) = 0;
  };

}; //namespace libargvcodec
