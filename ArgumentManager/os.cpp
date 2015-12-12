#include "os.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "psapi.h"
#pragma comment(lib,"psapi.lib")


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
