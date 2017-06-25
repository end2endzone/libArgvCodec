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


#include "os.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "psapi.h"
#pragma comment(lib,"psapi.lib")

namespace libargvcodec
{

std::string getLastErrorDescription()
{
  DWORD dwLastError = ::GetLastError();
  char lpErrorBuffer[10240] = {0};
  DWORD dwErrorBufferSize = sizeof(lpErrorBuffer);
  if(dwLastError != 0)    // Don't want to see a "operation done successfully" error ;-)
  {
    ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
                     NULL,                                      // No string to be formatted needed
                     dwLastError,                               // Hey Windows: Please explain this error!
                     MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),  // Do it in the standard language
                     lpErrorBuffer,                             // Put the message here
                     dwErrorBufferSize-1,                       // Number of bytes to store the message
                     NULL);
    char lpDescBuffer[10240] = {0};
    sprintf(lpDescBuffer, "Error %d, %s", dwLastError, lpErrorBuffer);
    return std::string(lpDescBuffer);
  }
  return std::string();
}

std::string getLocalDllPath()
{
  std::string path;
  char buffer[MAX_PATH] = {0};
  HMODULE hModule = NULL;
  if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
          GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
          (LPCSTR) /*&getLocalExeName*/ __FUNCTION__,
          &hModule))
  {
    return getLastErrorDescription();
  }
  /*get the path of this DLL*/
  GetModuleFileName(hModule, buffer, sizeof(buffer));
  if (buffer[0] != '\0')
  {
    /*remove .dll from path*/
    buffer[std::string(buffer).size() - 4] = '\0';
    path = buffer;
  }
  return path;
}

std::string getLocalExePath()
{
  std::string path;
  //HANDLE hProcess = OpenProcess(
  //  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
  //  FALSE,
  //  ::GetCurrentProcessId()
  //);
  HANDLE hProcess = GetCurrentProcess();
  if (hProcess) 
  {
    char buffer[MAX_PATH] = {0};
    if (GetModuleFileNameEx(hProcess, 0, buffer, MAX_PATH))
    {
      path = buffer;
    }
    else
    {
      return getLastErrorDescription();
    }
    CloseHandle(hProcess);
  }
  return path;
}

}; //namespace libargvcodec
