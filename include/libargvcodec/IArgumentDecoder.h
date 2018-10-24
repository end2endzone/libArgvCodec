#ifndef IARGUMENTDECODER_H
#define IARGUMENTDECODER_H

#include "libargvcodec/config.h"
#include "ArgumentList.h"

namespace libargvcodec
{

  class LIBARGVCODEC_EXPORT IArgumentDecoder
  {
  public:

    /// <summary>Decodes a single argument.</summary>
    /// <param name="iValue">The encoded argument</param>
    /// <returns>Returns the value of the argument without all encoding characters.</returns>
    virtual std::string decodeArgument(const char * iValue) = 0;

    /// <summary>Decodes a command line into a list of arguments.</summary>
    /// <remarks>The function automatically adds the current executable path as the first argument of the list.</remarks>
    /// <param name="iValue">The command line string.</param>
    /// <returns>Returns all argument's value removing all encoding characters.</returns>
    virtual ArgumentList decodeCommandLine(const char * iValue) = 0;
  };

}; //namespace libargvcodec

#endif //IARGUMENTDECODER_H
