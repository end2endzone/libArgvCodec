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


#include "utils.h"

namespace libargvcodec
{
namespace utils
{

void strReplace(std::string & iValue, const std::string & iOldStr, const std::string & iNewStr)
{
  size_t pos = 0;
  while((pos = iValue.find(iOldStr, pos)) != std::string::npos)
  {
    iValue.replace(pos, iOldStr.length(), iNewStr);
    pos += iNewStr.length();
  }
}

void strReplace(std::string & iValue, const char * iOldStr, const char * iNewStr)
{
  size_t oldLength = std::string(iOldStr).length();
  size_t newLength = std::string(iNewStr).length();
  size_t pos = 0;
  while((pos = iValue.find(iOldStr, pos)) != std::string::npos)
  {
    iValue.replace(pos, oldLength, iNewStr);
    pos += newLength;
  }
}

size_t findNumTrailingBackslashes(const char * iValue)
{
  if (iValue == NULL)
    return 0;

  size_t numTrailingBackslashes = 0;
  size_t len = std::string(iValue).size();
  for(size_t i=0; i<len; i++)
  {
    char c = iValue[i];
    if (c == '\\')
      numTrailingBackslashes++;
    else
      numTrailingBackslashes = 0;
  }
  return numTrailingBackslashes;
}

std::string uppercase(const std::string & iValue)
{
  std::string out;
  for(size_t i=0; i<iValue.size(); i++)
  {
    char c = (char)toupper(iValue[i]);
    out.append(1, c);
  }
  return out;
}

std::string uppercase(const char * iValue)
{
  if (iValue == NULL)
    return "";
  return uppercase(std::string(iValue));
}

}; //namespace utils
}; //namespace libargvcodec
