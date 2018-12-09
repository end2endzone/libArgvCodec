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
