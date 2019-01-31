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
