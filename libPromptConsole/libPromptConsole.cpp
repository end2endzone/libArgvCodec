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
