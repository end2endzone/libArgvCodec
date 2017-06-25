//
//  libArgvCodec Library - v1.0 - 06/25/2017
//  Copyright (C) 2017 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  
//  The libArgvCodec library is used when an application has to launch another one.
//  It includes arguments and command line encoder/decoder (codecs) that are
//  designed for the purpose of helping a developper in this situation. The
//  libArgvCodec library is designed to avoid typical encoding and decoding pitfalls
//  when transferring local variable values to other application through command
//  line arguments.
//
// HISTORY:
// 06/25/2017 v1.0 - Initial release.
//


#pragma once

#include "IArgumentEncoder.h"
#include "IArgumentDecoder.h"

namespace libargvcodec
{

  class LIBARGVCODEC_API CmdPromptArgumentCodec : public virtual IArgumentEncoder,
    public virtual IArgumentDecoder
  {
  public:
    CmdPromptArgumentCodec();
    virtual ~CmdPromptArgumentCodec();

    //IArgumentEncoder
    virtual std::string encodeArgument(const char * iValue);
    virtual std::string encodeCommandLine(const ArgumentList & iArguments);

    //IArgumentDecoder
    virtual std::string decodeArgument(const char * iValue);
    virtual ArgumentList decodeCommandLine(const char * iValue);

  public:
    virtual bool isArgumentSeparator(const char c);
    virtual bool isShellCharacter(const char c);
    virtual bool hasShellCharacters(const char * iValue);
    virtual bool supportsShellCharacters();

  protected:
    bool parseCmdLine(const char * iCmdLine, ArgumentList::StringList & oArguments);
    char getSafeCharacter(const char * iValue, size_t iIndex);
    bool matchesSequence(const char * iValue, const char * iSequenceExpr);
    bool matchesSequence(const char * iValue, size_t iValueOffset, const char * iSequenceExpr);
    bool matchesBackSlashDblQuoteSequence(const char * iValue, size_t iValueOffset, size_t & oNumBlackSlash, size_t & oSkipLength, bool iInString, bool iInCaretString);
    bool isStringStart(const char * iCmdLine, size_t iOffset);
    bool isStringEnd(const char * iCmdLine, size_t iOffset, size_t iSequenceLength);

  };

}; //namespace libargvcodec
