#include "TestUtils.h"
#include <cstdio> //for sprintf()
#include <string.h> //for strdup()
#include <cstdlib> //for free()

#include "rapidassist/random.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/process.h"
#include "rapidassist/gtesthelp.h"

//CreateProcess() unit test support
#ifdef _WIN32
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#   endif /* WIN32_LEAN_AND_MEAN */
#   include <Windows.h>
#endif

DynamicStringList gStrings;

//register a new string for the current test
char * mkstr(const char * value)
{
#ifdef _WIN32
  char * _copy = _strdup(value);
#else
  char * _copy = strdup(value);
#endif
  gStrings.push_back(_copy);
  return _copy;
}

void clearDynamicStrings()
{
  //clear all strings created in the test
  for(size_t i=0; i<gStrings.size(); i++)
  {
    char * value = gStrings[i];
    free(value);
  }
  gStrings.clear();
}

bool isDashedLine(const std::string & iLine)
{
  if (iLine.size() < 10) //must be at least 10 dash characters long
    return false;

  for(size_t i=0; i<iLine.size(); i++)
  {
    if (iLine[i] != '-')
      return false; //not a dash character
  }

  return true;
}

std::string getSequencedFile(const char * iPrefix, int iValue, const char * iPostfix, int iValueLength)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];

  char valueFormat[BUFFER_SIZE];
  char value[BUFFER_SIZE];
  sprintf(valueFormat, "%%0%dd", iValueLength);
  sprintf(value, valueFormat, iValue);

  sprintf(buffer, "%s%s%s", iPrefix, value, iPostfix);

  return std::string(buffer);
}

std::string getShowArgsExecutablePath()
{
  std::string path = ra::process::getCurrentProcessPath();
  int numTokenReplaced = ra::strings::replace(path, "libargvcodec_unittest", "showargs");
  return path;
}

bool getArgumentsFromSystem(const std::string & iCmdLineString, ra::strings::StringVector & oArguments)
{
  oArguments.clear();

  //define a tmp file and a batch file for testing
  const std::string file_prefix = "showargs.";
  std::string random_string = ra::random::getRandomString(10);
  std::string tmp_file    = file_prefix + random_string + std::string(".output.tmp");
#if defined(_WIN32)
  const std::string call_method = "call ";
  std::string script_file = file_prefix + random_string + std::string(".script.bat");
#elif defined(__linux__)
  const std::string call_method = "source ";
  std::string script_file = file_prefix + random_string + std::string(".script.sh");
#endif

  //build the script file
  FILE * f = fopen(script_file.c_str(), "w");
  if (!f)
  {
    printf("Unable to create script file '%s'\n", script_file.c_str());
    return false;
  }
#if defined(_WIN32)
  fprintf(f, "@echo off\n");
  fprintf(f, "\"%s\" %s\n", getShowArgsExecutablePath().c_str(), iCmdLineString.c_str());
#elif defined(__linux__)
  fprintf(f, "./\"%s\" %s\n", getShowArgsExecutablePath().c_str(), iCmdLineString.c_str());
#endif
  fclose(f);

  //build command line
  std::string cmdline;
  cmdline = call_method;
  cmdline += script_file;
  cmdline += " >>";
  cmdline += tmp_file;

  //delete the tmp file if it already exists
  if (ra::filesystem::fileExists(tmp_file.c_str()))
  {
    bool deleted = ra::filesystem::deleteFile(tmp_file.c_str());
    if (!deleted)
    {
      printf("Failed deleting file '%s'\n", tmp_file.c_str());
      return false;
    }
  }

  //execute the command line
  int return_code = system(cmdline.c_str());
  if (return_code != 0)
  {
    printf("Failed executing command line:%s\n", cmdline.c_str());
    return false;
  }

  //expect the tmp file was created
  if (!ra::filesystem::fileExists(tmp_file.c_str()))
  {
    printf("Command line '%s' failed to produce an output in file '%s'\n", cmdline.c_str(), tmp_file.c_str());
    return false;
  }

  //parse 'showargs' output
  ra::strings::StringVector system_arguments;
  bool argumentParsed = ra::gtesthelp::getTextFileContent(tmp_file.c_str(), system_arguments);
  if (!argumentParsed)
  {
    printf("Failed parsing file '%s'\n", tmp_file.c_str());
    return false;
  }

  //cleanup "argX: " line prefix.
  for(size_t j=0; j<system_arguments.size(); j++)
  {
    std::string prefix = "arg" + ra::strings::toString(j) + ": ";
    std::string & actual_argument = system_arguments[j];
    int numTokenReplaced = ra::strings::replace(actual_argument, prefix.c_str(), "");
  }

  //remove arg[0] from the list
  if (system_arguments.size() > 0)
  {
    system_arguments.erase(system_arguments.begin());
  }

  //delete the tmp file
  if (ra::filesystem::fileExists(tmp_file.c_str()))
  {
    bool deleted = ra::filesystem::deleteFile(tmp_file.c_str());
    if (!deleted)
    {
      printf("Failed deleting file '%s'\n", tmp_file.c_str());
      return false;
    }
  }

  //delete the script file
  if (ra::filesystem::fileExists(script_file.c_str()))
  {
    bool deleted = ra::filesystem::deleteFile(script_file.c_str());
    if (!deleted)
    {
      printf("Failed deleting file '%s'\n", script_file.c_str());
      return false;
    }
  }

  oArguments = system_arguments;
  return true;
}

bool getArgumentsFromCreateProcess(const std::string & iCmdline, ra::strings::StringVector & oArguments)
{
  oArguments.clear();

#ifndef _WIN32
  return false; // CreateProcess() API is not available.
#else

  //build a command using createprocess
  // http://stackoverflow.com/questions/19398606/how-to-include-argument-with-createprocess-api-in-vc
  // http://stackoverflow.com/questions/1135784/createprocess-doesnt-pass-command-line-arguments
  // https://stackoverflow.com/questions/7018228/how-do-i-redirect-output-to-a-file-with-createprocess
  // https://docs.microsoft.com/en-us/windows/desktop/ProcThread/creating-a-child-process-with-redirected-input-and-output

  //define a tmp file and a batch file for testing
  const std::string file_prefix = "showargs.";
  std::string random_string = ra::random::getRandomString(10);
  std::string tmp_file    = file_prefix + random_string + std::string(".output.tmp");

  //build command line
  static const int CMDLINE_SIZE = 20480;
  char cmdline[CMDLINE_SIZE];
  sprintf(cmdline, "%s %s", getShowArgsExecutablePath().c_str(), iCmdline.c_str());

  //delete the tmp file if it already exists
  if (ra::filesystem::fileExists(tmp_file.c_str()))
  {
    bool deleted = ra::filesystem::deleteFile(tmp_file.c_str());
    if (!deleted)
    {
      printf("Failed deleting file '%s'\n", tmp_file.c_str());
      return false;
    }
  }

  //execute the command line
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(sa);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;

  HANDLE hFileOut = CreateFile(tmp_file.c_str(),
      FILE_APPEND_DATA,
      FILE_SHARE_WRITE | FILE_SHARE_READ,
      &sa,
      OPEN_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL );
  if ( hFileOut == INVALID_HANDLE_VALUE )
    return false;

  PROCESS_INFORMATION pi;
  STARTUPINFO si;
  BOOL bProcessCreated = FALSE;
  DWORD flags = 0; //CREATE_NO_WINDOW;

  ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );
  ZeroMemory( &si, sizeof(STARTUPINFO) );
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags |= STARTF_USESTDHANDLES;
  si.hStdInput = NULL;
  si.hStdError = hFileOut;
  si.hStdOutput = hFileOut;

  if (CreateProcess(getShowArgsExecutablePath().c_str(), cmdline, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi))
  {
    //wait for the process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
  }  
  else
  {
    //unable to start the process
    return false;
  }

  //close file outout handle
  if ( ! CloseHandle(hFileOut) ) 
  {
    printf("Failed closing output handle of file '%s'\n", tmp_file.c_str());
    return false;
  }

  //expect the tmp file was created
  if (!ra::filesystem::fileExists(tmp_file.c_str()))
  {
    printf("Command line '%s' failed to produce an output in file '%s'\n", cmdline, tmp_file.c_str());
    return false;
  }

  //parse 'showargs' output
  ra::strings::StringVector system_arguments;
  bool argumentParsed = ra::gtesthelp::getTextFileContent(tmp_file.c_str(), system_arguments);
  if (!argumentParsed)
  {
    printf("Failed parsing file '%s'\n", tmp_file.c_str());
    return false;
  }

  //cleanup "argX: " line prefix.
  for(size_t j=0; j<system_arguments.size(); j++)
  {
    std::string prefix = "arg" + ra::strings::toString(j) + ": ";
    std::string & actual_argument = system_arguments[j];
    int numTokenReplaced = ra::strings::replace(actual_argument, prefix.c_str(), "");
  }

  //remove arg[0] from the list
  if (system_arguments.size() > 0)
  {
    system_arguments.erase(system_arguments.begin());
  }

  //delete the tmp file
  if (ra::filesystem::fileExists(tmp_file.c_str()))
  {
    bool deleted = ra::filesystem::deleteFile(tmp_file.c_str());
    if (!deleted)
    {
      printf("Failed deleting file '%s'\n", tmp_file.c_str());
      return false;
    }
  }

  oArguments = system_arguments;
  return true;
#endif //ifndef _WIN32
}

ra::strings::StringVector toStringList(const libargvcodec::ArgumentList & arguments)
{
  ra::strings::StringVector list;

  for(int i=0; i<arguments.getArgc(); i++)
  {
    const char * arg = arguments.getArgument(i);
    list.push_back(arg);
  }

  //remove arg[0] from the list
  if (list.size() > 0)
  {
    list.erase(list.begin());
  }

  return list;
}

std::string buildErrorString(const std::string & iVar1, const ra::strings::StringVector & iList1, const std::string & iVar2, const ra::strings::StringVector & iList2)
{
  std::string desc;

  desc += "The content of '" + iVar1 + "' which is:\n";
  for(size_t j=0; j<iList1.size(); j++)
  {
    desc += "  arg[" + ra::strings::toString(j) + "]: ";
    desc += iList1[j] + "\n";
  }
  desc += "does not match the content of '" + iVar2 + "' which is:\n";
  for(size_t j=0; j<iList2.size(); j++)
  {
    desc += "  arg[" + ra::strings::toString(j) + "]: ";
    desc += iList2[j] + "\n";
  }

  return desc;
}

std::string buildErrorString(const std::string & iCmdLine, const ra::strings::StringVector & iExpectedArguments, const ra::strings::StringVector & iActualArguments)
{
  std::string desc;

  desc += "The command line `" + iCmdLine + "`\n";
  desc += "was expected to return the following arguments:\n";
  for(size_t j=0; j<iExpectedArguments.size(); j++)
  {
    desc += "  arg[" + ra::strings::toString(j) + "]: ";
    desc += iExpectedArguments[j] + "\n";
  }
  desc += "but actually returned the following arguments:\n";
  for(size_t j=0; j<iActualArguments.size(); j++)
  {
    desc += "  arg[" + ra::strings::toString(j) + "]: ";
    desc += iActualArguments[j] + "\n";
  }

  return desc;
}

std::string buildErrorString(const ra::strings::StringVector & iArguments, const std::string & iActualCmdLine, const ra::strings::StringVector & iActualArguments)
{
  std::string desc;

  desc += "The following arugments:\n";
  for(size_t j=0; j<iArguments.size(); j++)
  {
    desc += "  arg[" + ra::strings::toString(j) + "]: ";
    desc += iArguments[j] + "\n";
  }
  desc += "was encoded into the actual command line `" + iActualCmdLine + "`\n";
  desc += "which decoded returns the following arguments:\n";
  for(size_t j=0; j<iActualArguments.size(); j++)
  {
    desc += "  arg[" + ra::strings::toString(j) + "]: ";
    desc += iActualArguments[j] + "\n";
  }

  return desc;
}

bool loadCommandLineTestFile(const std::string & iPath, TEST_DATA_LIST & oItems)
{
  oItems.clear();

  if (!ra::filesystem::fileExists(iPath.c_str()) )
    return false;

  //load file in memory
  ra::strings::StringVector content;
  bool readSuccess = ra::gtesthelp::getTextFileContent(iPath.c_str(), content);
  if (!readSuccess)
    return false;

  //process each command lines
  ra::strings::StringVector commands;
  for(size_t i=0; i<content.size(); i++)
  {
    const std::string & line = content[i];
    if (isDashedLine(line))
    {
      //process current commands
      if (commands.size() >= 2) //must be at least 2 lines long (a cmdline and at least 1 argument)
      {
        //get command line and arguments from the file:
        std::string file_cmdline = commands[0];
        commands.erase(commands.begin());
        ra::strings::StringVector file_arguments = commands;

        //create a new item
        TEST_DATA td;
        td.cmdline = file_cmdline;
        td.arguments = file_arguments;
        oItems.push_back(td);
      }

      //this command is completed
      commands.clear();
    }
    else
    {
      commands.push_back(line); //that is a command line string or an argument string
    }
  }

  return true;
}
