// ArgMgr.cpp : Defines the exported functions for the DLL application.
//

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "psapi.h"
#pragma comment(lib,"psapi.lib")

#include "ArgumentManager.h"


#define SAFE_DELETE(var)        {if (var) delete   var; var = NULL;}
#define SAFE_DELETE_ARRAY(var)  {if (var) delete[] var; var = NULL;}

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

ArgumentManager::ArgumentManager() :
  mArgv(NULL)
{
}

ArgumentManager::ArgumentManager(const ArgumentManager & iArgumentManager) :
  mArgv(NULL)
{
  (*this) = iArgumentManager;
}

ArgumentManager::~ArgumentManager()
{
  SAFE_DELETE_ARRAY(mArgv);
}

void ArgumentManager::init(int argc, char** argv)
{
  StringList tmp;
  for(int i=0; i<argc; i++)
  {
    tmp.push_back(argv[i]);
  }
  init(tmp);
}

void ArgumentManager::init(const char * iCmdLine)
{
  init(iCmdLine, true);
}

void ArgumentManager::init(const char * iCmdLine, bool iIncludeExec)
{
  StringList args;

  bool parseSuccess = parseCmdLine(iCmdLine, args);
  if (!parseSuccess)
    args.clear();

  //insert .exe path
  if (iIncludeExec)
  {
    args.insert( args.begin(), getLocalExePath() );
  }

  init(args);
}

void ArgumentManager::init(const StringList & iArguments)
{
  mArguments = iArguments;
  rebuildArgv();
}

char * ArgumentManager::getArgument(int iIndex)
{
  if (!isValid(iIndex))
    return ""; //out of bounds
  return mArgv[iIndex];
}

bool ArgumentManager::insert(int iIndex, const char * iValue)
{
  if (iValue == NULL)
    return false;
  if (iIndex == getArgc())
    return insert(iValue); //insert at the end
  if (!isValid(iIndex))
    return false; //out of bounds

  mArguments.insert(mArguments.begin() + iIndex, iValue);
  rebuildArgv();
  return true;
}

bool ArgumentManager::insert(const char * iValue)
{
  if (iValue == NULL)
    return false;
  mArguments.push_back(iValue);
  rebuildArgv();
  return true;
}

bool ArgumentManager::remove(int iIndex)
{
  if (!isValid(iIndex))
    return false; //out of bounds
  mArguments.erase(mArguments.begin() + iIndex);
  rebuildArgv();
  return true;
}

bool ArgumentManager::replace(int iIndex, const char * iValue)
{
  if (iValue == NULL)
    return false;
  if (!isValid(iIndex))
    return false; //out of bounds

  std::string & value = mArguments[iIndex];
  value = iValue;

  rebuildArgv();
  return true;
}

int ArgumentManager::getArgc()
{
  return (int)mArguments.size();
}

char** ArgumentManager::getArgv()
{
  return mArgv;
}

const ArgumentManager & ArgumentManager::operator = (const ArgumentManager & iArgumentManager)
{
  this->init(iArgumentManager.mArguments);
  return (*this);
}

bool ArgumentManager::operator == (const ArgumentManager & iArgumentManager) const
{
  bool equals = (this->mArguments == iArgumentManager.mArguments);
  return equals;
}

bool ArgumentManager::operator != (const ArgumentManager & iArgumentManager) const
{
  bool notEquals = (this->mArguments != iArgumentManager.mArguments);
  return notEquals;
}

void ArgumentManager::rebuildArgv()
{
  SAFE_DELETE_ARRAY(mArgv);

  //argv size is 1 element bigger than argc (the number of arguments)
  //the last element of argv must be a NULL string
  //the last element is *not* an argument
  int argvSize = mArguments.size() + 1;

  typedef char * cStr;
  mArgv = new cStr[argvSize];
  if (mArgv)
  {
    //fill
    for(size_t i=0; i<mArguments.size(); i++)
    {
      mArgv[i] = (char*)mArguments[i].c_str();
    }
    //last vector element must be NULL
    mArgv[mArguments.size()] = NULL;
  }
}

bool ArgumentManager::isValid(int iIndex)
{
  if (iIndex >= 0 && iIndex < getArgc())
  {
    return true;
  }
  return false;
}

bool ArgumentManager::parseCmdLine(const char * iCmdLine, StringList & oArguments)
{
  oArguments.clear();

  std::string accumulator;
  std::string iCmdLineStr = iCmdLine;

  bool inString = false;

  char previous = '\0';

  for(size_t i=0; i<iCmdLineStr.size(); i++)
  {
    char c = iCmdLine[i];
    bool hasPreviousCharacter = (i>0);
    bool hasNextCharacter = (i<iCmdLineStr.size()-1);
    bool isLastCharacter = !hasNextCharacter;

    bool isQuoteStart = (c == '\"' && (i==0 || iCmdLine[i-1]==' '));

    if (c == '\"' && inString == false)
    {
      //starting a new string
      inString = true;
    }
    else if (c == '\"' && inString == true)
    {
      //endding a new string
      inString = false;

      bool forceFlush = ( !hasNextCharacter ||                        //is the last argument specified
                          (hasNextCharacter && iCmdLine[i+1] == ' ')  //matches the end of an argument
                          ); //deal with "" arguments

      //flush accumulator;
      if (accumulator != "" || forceFlush )
      {
        oArguments.push_back(accumulator);
        accumulator = "";
      }
    }
    else if ((c == ' ' || c == '\t') && inString == false)
    {
      //flush accumulator;
      if (accumulator != "")
      {
        oArguments.push_back(accumulator);
        accumulator = "";
      }
    }
    //else if (c == '\\' && inString == false)
    //{
    //  //do not escape character
    //  accumulator.push_back(c);
    //}

    //NEVER ESCAPE \\ as \
    //ie: foo.exe a\\b
    // argv[1]=a\\b
    //ie: foo.exe "a\\b"
    // argv[1]=a\\b
    //else if (c == '\\' && hasNextCharacter && iCmdLine[i+1] == '\\')
    //{
    //  //escaping next \ character
    //  accumulator.push_back('\\');
    //  i++; //skip escaped character
    //}

    //////////////////ESCAPE \\ only when they are within a string.
    //////////////////ie:   "a b\\" c d
    //////////////////argv[1]=a b\
    //////////////////argv[2]=c
    //////////////////argv[3]=d
    ////////////////else if (c == '\\' && hasNextCharacter && iCmdLine[i+1] == '\\' && inString == true)
    ////////////////{
    ////////////////  //escaping next \ character
    ////////////////  accumulator.push_back('\\');
    ////////////////  i++; //skip escaped character
    ////////////////}

    else if (c == '\\' && hasNextCharacter && iCmdLine[i+1] == '\"')
    {
      //escaping next " character
      accumulator.push_back('\"');
      i++; //skip escaped character
    }
    else
    {
      //normal character
      accumulator.push_back(c);
    }

    //remember previous
    previous = c;
  }

  //flush accumulator;
  if (accumulator != "")
  {
    oArguments.push_back(accumulator);
    accumulator = "";
  }

  return true;
}

int ArgumentManager::findIndex(const char * iValue)
{
  if (iValue == NULL)
    return -1;
  for(size_t i=0; i<mArguments.size(); i++)
  {
    const std::string & arg = mArguments[i];
    if (arg == iValue)
      return (int)i;
  }
  return -1;
}

bool ArgumentManager::contains(const char * iValue)
{
  if (iValue == NULL)
    return false;
  int pos = findIndex(iValue);
  return pos != -1;
}

bool ArgumentManager::findOption(const char * iValue)
{
  if (iValue == NULL)
    return false;
  int tmp = 0;
  return findOption(iValue, tmp);
}

bool ArgumentManager::findOption(const char * iValue, int & oIndex)
{
  oIndex = -1;
  if (iValue == NULL)
    return false;
  oIndex = findIndex(iValue);
  return oIndex != -1;
}

bool ArgumentManager::findValue(const char * iValueName, int & oIndex, std::string & oValue)
{
  oIndex = -1;
  oValue= "";
  if (iValueName == NULL)
    return false;
  for(size_t i=0; i<mArguments.size(); i++)
  {
    const std::string & arg = mArguments[i];
    std::string argPrefix = arg.substr(0, std::string(iValueName).size());
    if (argPrefix == iValueName)
    {
      oIndex = (int)i;
      oValue = arg.substr(argPrefix.size(), 999999);
      return true;
    }
  }
  return false;
}

bool ArgumentManager::findValue(const char * iValueName, int & oIndex, int & oValue)
{
  oIndex = -1;
  oValue = 0;

  int index = 0;
  std::string sValue;
  bool found = findValue(iValueName, index, sValue);
  if (!found)
    return false;
  if (sValue != "")
  {
    oValue = atoi(sValue.c_str());
  }
  return true;
}

bool ArgumentManager::extractOption(const char * iValue)
{
  if (iValue == NULL)
    return false;
  int index = 0;
  bool found = findOption(iValue, index);
  if (found)
  {
    return this->remove(index);
  }
  return false;
}

bool ArgumentManager::extractValue(const char * iValueName, std::string & oValue)
{
  oValue = "";
  if (iValueName == NULL)
    return false;
  int index = 0;
  bool found = findValue(iValueName, index, oValue);
  if (found)
  {
    return this->remove(index);
  }
  return false;
}

bool ArgumentManager::extractValue(const char * iValueName, int & oValue)
{
  oValue = 0;

  std::string sValue;
  bool found = extractValue(iValueName, sValue);
  if (!found)
    return false;
  if (sValue != "")
  {
    oValue = atoi(sValue.c_str());
  }
  return true;
}

std::string ArgumentManager::extractValue(const char * iValueName)
{
  if (iValueName == NULL)
    return "";
  int index = 0;
  std::string value;
  bool found = findValue(iValueName, index, value);
  if (found)
  {
    bool removed = this->remove(index);
    if (removed)
      return value;
    else
      return "";
  }
  return "";
}
