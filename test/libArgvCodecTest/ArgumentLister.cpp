#include "ArgumentLister.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "rapidassist/strings.h"

//CreateProcess() unit test support
#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif /* WIN32_LEAN_AND_MEAN */
#include <Windows.h>

#endif

using namespace libargvcodec;

std::string getExePath()
{
  CmdPromptArgumentCodec c;

  ArgumentList tmp = c.decodeCommandLine("");
  const char * arg0 = tmp.getArgument(0);
  return std::string(arg0);
}

bool isPrinterExec()
{
  std::string localExec = getExePath();
  bool found = (localExec.find(".printer.exe") != std::string::npos);
  return found;
}

bool isLoggerExec()
{
  std::string localExec = getExePath();
  bool found = (localExec.find(".logger.exe") != std::string::npos);
  return found;
}

std::string getPrinterExecFilePath()
{
  std::string localExec = getExePath();

  if (isPrinterExec())
    return localExec;

  std::string printerExec = localExec;
  ra::strings::replace(printerExec, ".exe", ".printer.exe");
  return printerExec;
}

std::string getLoggerExecFilePath()
{
  std::string localExec = getExePath();

  if (isLoggerExec())
    return localExec;

  std::string loggerExec = localExec;
  ra::strings::replace(loggerExec, ".exe", ".logger.exe");
  return loggerExec;
}

std::string getLogFilePath()
{
  std::string logPath = getLoggerExecFilePath();
  ra::strings::replace(logPath, ".exe", ".log");
  return logPath;
}

void duplicateExec(const char * iDestinationPath)
{
  std::string localExec = getExePath();

  printf("Duplicating to \"%s\"... ", iDestinationPath);
  
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  sprintf(buffer, "copy /v \"%s\" \"%s\" >NUL 2>NUL", localExec.c_str(), iDestinationPath);
  system(buffer);

  printf("done\n");
}

/*
 Description:
  Copy itself as *.printer.exe and *.logger.exe to properly detect between the following behavior:
    1. Run all tests cases
    2. Print given arguments to the screen.
    3. Log given arguments to a fileexecuting test cases, printing arguments
*/
void initExecStubs()
{
  duplicateExec( getPrinterExecFilePath().c_str() );
  duplicateExec( getLoggerExecFilePath().c_str() );
}

int printArgs(int argc, char* argv[])
{
  for(int i=0; i<argc; i++)
  {
    std::string arg = argv[i];
    
    //replace tabs by [tabs] code since they are not visible on the console.
    ra::strings::replace(arg, "\t", "[tabs]");

    //since arguments that ends with spaces will not be seen properly on the console,
    //show the argument enclosed in ().
    bool showParenthesis = (arg.size() > 0 && arg[arg.size()-1] == ' ');

    if (!showParenthesis)
      printf("argv[%d]=%s\n", i, arg.c_str());
    else
      printf("argv[%d]=(%s) without enclosing parenthesis\n", i, arg.c_str());
  }
  return 0;
}

void debugValues()
{
  std::string logPath         = getLogFilePath();
  std::string localExec       = getExePath();
  std::string loggerExecPath  = getLoggerExecFilePath();
  std::string printerExecPath = getPrinterExecFilePath();
#define printstring(expr) printf("%s=\"%s\"\n", #expr, expr.c_str());
  printstring(logPath        );
  printstring(localExec      );
  printstring(loggerExecPath );
  printstring(printerExecPath);
}

int logArgs(int argc, char* argv[])
{
  std::string logPath = getLogFilePath();

  FILE * f = fopen(logPath.c_str(), "w");
  if (!f)
    return 1;
  for(int i=0; i<argc; i++)
  {
    fprintf(f, "argv[%d]=%s\n", i, argv[i]);
  }
  fclose(f);
  return 0;
}

std::string systemDecodeArgument(const char * iValue)
{
  ArgumentList::StringList arguments;
  bool success = systemDecodeCommandLineArguments(iValue, arguments);
  if (arguments.size() != 2)
    return "";

  std::string confirmedArgumentValue = arguments[1];
  return confirmedArgumentValue;
}

bool systemDecodeCommandLineArguments(const char * iCmdLine, ArgumentList::StringList & oArguments)
{
  oArguments.clear();

  //build a command line to list arguments
  std::string loggerExecPath = getLoggerExecFilePath();
  
  //build command line
  std::string cmdLine;
  cmdLine.append(loggerExecPath);
  cmdLine.append(" ");
  cmdLine.append(iCmdLine);

  //run command line
  system(cmdLine.c_str());

  std::string logPath = getLogFilePath();
  FILE * f = fopen(logPath.c_str(), "r");
  if (!f)
    return false;

  static const int BUFFER_SIZE = 102400;
  char buffer[BUFFER_SIZE];

  int argNumber = 0;
  while( fgets(buffer, BUFFER_SIZE, f) != NULL )
  {
    //expected to read: arg0=test
    std::string line = buffer;

    //build arg template
    sprintf(buffer, "argv[%d]=", argNumber);

    //remove from line;
    ra::strings::replace(line, buffer, "");

    //remove ending CRLF at the end of the line;
    ra::strings::replace(line, "\n", "");

    oArguments.push_back(line);

    argNumber++;
  }

  fclose(f);
  return true;
}

bool createProcessDecodeCommandLineArguments(const char * iCmdLine, ArgumentList::StringList & oArguments)
{
  oArguments.clear();

#ifndef _WIN32
  return true; // CreateProcess() API is not available.
#else
  //build a command line to list arguments
  std::string loggerExecPath = getLoggerExecFilePath();
  
  //build a command using createprocess
  // http://stackoverflow.com/questions/19398606/how-to-include-argument-with-createprocess-api-in-vc
  // http://stackoverflow.com/questions/1135784/createprocess-doesnt-pass-command-line-arguments

  //Build command line
  //The command line should include argv[0] (the path of the executable)
  //The command line (second) parameter of CreateProcess() should be a pointer to modifiable memory
  static const int CMDLINE_SIZE = 33000;
  char cmdLine[CMDLINE_SIZE];
  sprintf(cmdLine, "%s %s", loggerExecPath.c_str(), iCmdLine);

  PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
  STARTUPINFO StartupInfo; //This is an [in] parameter

  ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
  
  ZeroMemory(&StartupInfo, sizeof(StartupInfo));
  StartupInfo.cb = sizeof StartupInfo ; //Only compulsory field

  if(CreateProcess(loggerExecPath.c_str(), cmdLine, 
      NULL,NULL,FALSE,0,NULL,
      NULL,&StartupInfo,&ProcessInfo))
  {
    //wait for the process to finish
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    CloseHandle(ProcessInfo.hThread);
    CloseHandle(ProcessInfo.hProcess);
  }  
  else
  {
    //unable to start the process
    return false;
  }

  std::string logPath = getLogFilePath();
  FILE * f = fopen(logPath.c_str(), "r");
  if (!f)
    return false;

  static const int BUFFER_SIZE = 102400;
  char buffer[BUFFER_SIZE];

  int argNumber = 0;
  while( fgets(buffer, BUFFER_SIZE, f) != NULL )
  {
    //expected to read: arg0=test
    std::string line = buffer;

    //build arg template
    sprintf(buffer, "argv[%d]=", argNumber);

    //remove from line;
    ra::strings::replace(line, buffer, "");

    //remove ending CRLF at the end of the line;
    ra::strings::replace(line, "\n", "");

    oArguments.push_back(line);

    argNumber++;
  }

  fclose(f);
  return true;
#endif //ifndef _WIN32
}

std::string createProcessDecodeArgument(const char * iValue)
{
  ArgumentList::StringList arguments;
  bool success = createProcessDecodeCommandLineArguments(iValue, arguments);
  if (arguments.size() != 2)
    return "";

  std::string confirmedArgumentValue = arguments[1];
  return confirmedArgumentValue;
}
