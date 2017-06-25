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


#include "libPromptConsole.h"
#include <cstdlib>
#include <iostream>

#ifndef WIN32_LEAN_AND_MEAN
  //exclude rarely used stuff
  #define WIN32_LEAN_AND_MEAN
#endif /* WIN32_LEAN_AND_MEAN */

#if       _WIN32_WINNT < 0x0500
  #undef  _WIN32_WINNT
  #define _WIN32_WINNT   0x0500
#endif
#include <windows.h>
#include "Wincon.h" 

bool isPromptConsoleWindow()
{
  const char* prompt = std::getenv("PROMPT");
  if (prompt != NULL)
  {
    return true;
  }
  return false;
}

bool hasConsoleOwnership()
{
  //https://stackoverflow.com/questions/9009333/how-to-check-if-the-program-is-run-from-a-console

  HWND consoleWnd = GetConsoleWindow();
  DWORD dwConsoleProcessId = 0;
  GetWindowThreadProcessId(consoleWnd, &dwConsoleProcessId);
  DWORD dwCurrentProcessId = GetCurrentProcessId();
  GetWindowThreadProcessId(consoleWnd, &dwConsoleProcessId);
  if (dwCurrentProcessId==dwConsoleProcessId)
  {
    return true;
  }
  return false;
}
