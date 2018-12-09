/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

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
