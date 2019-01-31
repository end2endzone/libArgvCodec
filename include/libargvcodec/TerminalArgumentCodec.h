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

#ifndef TERMINALARGUMENTCODEC_H
#define TERMINALARGUMENTCODEC_H

#include "IArgumentEncoder.h"
#include "IArgumentDecoder.h"

namespace libargvcodec
{

  class LIBARGVCODEC_EXPORT TerminalArgumentCodec : public virtual IArgumentEncoder,
    public virtual IArgumentDecoder
  {
  public:
    TerminalArgumentCodec();
    virtual ~TerminalArgumentCodec();

    //IArgumentEncoder
    virtual std::string encodeArgument(const char * iValue);
    virtual std::string encodeCommandLine(const ArgumentList & iArguments);

    //IArgumentDecoder
    virtual std::string decodeArgument(const char * iValue);
    virtual ArgumentList decodeCommandLine(const char * iValue);

  public:
    /// <summary>Returns true if the given character is an argument separator character.</summary>
    /// <param name="c">The given character to test.</param>
    /// <returns>Returns true if the given character is an argument separator character. Returns false otherwise.</returns>
    virtual bool isArgumentSeparator(const char c);

    /// <summary>Returns true if the given character is a shell character.</summary>
    /// <param name="c">The given character to test.</param>
    /// <returns>Returns true if the given character is a shell character. Returns false otherwise.</returns>
    virtual bool isShellCharacter(const char c);

    /// <summary>Returns true if the given string has at least one shell character.</summary>
    /// <param name="iValue">The given string to test. Must not be NULL.</param>
    /// <returns>Returns true if the given string has at least one shell character. Returns false otherwise.</returns>
    virtual bool hasShellCharacters(const char * iValue);

  protected:
    /// <summary>Parses a command line string into a list of arguments.</summary>
    /// <param name="iCmdLine">The command line string to parse.</param>
    /// <param name="oArguments">The output list of arguments.</param>
    /// <returns>Returns true on parse success. Returns false otherwise.</returns>
    bool parseCmdLine(const char * iCmdLine, ArgumentList::StringList & oArguments);

    /// <summary>Returns the character at offset iIndex of the given string. The function is safe as it returns \0 if iIndex is out-of-range.</summary>
    /// <param name="iValue">The given string.</param>
    /// <param name="iIndex">The character offset within the given iValue string.</param>
    /// <returns>Returns the character at offset iIndex of the given string. The function is safe as it returns \0 if iIndex is out-of-range.</returns>
    char getSafeCharacter(const char * iValue, size_t iIndex);

  };

}; //namespace libargvcodec

#endif //TERMINALARGUMENTCODEC_H
