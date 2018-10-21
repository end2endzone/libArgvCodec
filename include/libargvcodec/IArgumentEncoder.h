#ifndef IARGUMENTENCODER_H
#define IARGUMENTENCODER_H

#include "libargvcodec/config.h"
#include "ArgumentList.h"

namespace libargvcodec
{

  class LIBARGVCODEC_EXPORT IArgumentEncoder
  {
  public:
    //
    // Description:
    //  Encodes a single argument
    // Parameters:
    //  iValue:    The value of the argument
    //
    virtual std::string encodeArgument(const char * iValue) = 0;

    //
    // Description:
    //  Encodes an argument's list into a command line
    // Parameters:
    //  iArguments:    The list of arguments
    //
    virtual std::string encodeCommandLine(const ArgumentList & iArguments) = 0;
  };

}; //namespace libargvcodec

#endif //IARGUMENTENCODER_H
