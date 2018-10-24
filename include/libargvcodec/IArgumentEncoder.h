#ifndef IARGUMENTENCODER_H
#define IARGUMENTENCODER_H

#include "libargvcodec/config.h"
#include "ArgumentList.h"

namespace libargvcodec
{

  class LIBARGVCODEC_EXPORT IArgumentEncoder
  {
  public:
    /// <summary>Encodes a single argument.</summary>
    /// <param name="iValue">The value of the argument.</param>
    /// <returns>Returns the argument's value properly encoded.</returns>
    virtual std::string encodeArgument(const char * iValue) = 0;

    /// <summary>Encodes an ArgumentList (list of arguments) into a command line.</summary>
    /// <param name="iArguments">The list of arguments</param>
    /// <returns>Returns a command line strings with each single argument properly encoded.</returns>
    virtual std::string encodeCommandLine(const ArgumentList & iArguments) = 0;
  };

}; //namespace libargvcodec

#endif //IARGUMENTENCODER_H
