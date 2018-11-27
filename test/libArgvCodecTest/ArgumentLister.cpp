#include "ArgumentLister.h"
#include "TestUtils.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "rapidassist/strings.h"
#include "rapidassist/process.h"

#include <cstdlib> //for system()

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
  std::string path = ra::process::getCurrentProcessPath();
  return path;
}

bool isPrinterExec()
{
  std::string localExec = getExePath();
#ifdef __linux__
  bool found = (localExec.find(".printer") != std::string::npos);
#elif WIN32
  bool found = (localExec.find(".printer.exe") != std::string::npos);
#endif
  return found;
}

bool isLoggerExec()
{
  std::string localExec = getExePath();
#ifdef __linux__
  bool found = (localExec.find(".logger") != std::string::npos);
#elif WIN32
  bool found = (localExec.find(".logger.exe") != std::string::npos);
#endif
  return found;
}

std::string getPrinterExecFilePath()
{
  std::string localExec = getExePath();

  if (isPrinterExec())
    return localExec;

  std::string printerExec = localExec;
#ifdef __linux__
  printerExec.append(".printer");
#elif WIN32
  ra::strings::replace(printerExec, ".exe", ".printer.exe");
#endif
  return printerExec;
}

std::string getLoggerExecFilePath()
{
  std::string localExec = getExePath();

  if (isLoggerExec())
    return localExec;

  std::string loggerExec = localExec;
#ifdef __linux__
  loggerExec.append(".logger");
#elif WIN32
  ra::strings::replace(loggerExec, ".exe", ".logger.exe");
#endif
  return loggerExec;
}

std::string getLogFilePath()
{
  std::string logPath = getLoggerExecFilePath();
#ifdef __linux__
  logPath.append(".log");
#elif WIN32
  ra::strings::replace(logPath, ".exe", ".log");
#endif
  return logPath;
}

void duplicateExec(const char * iDestinationPath)
{
  std::string localExec = getExePath();

  printf("Duplicating to \"%s\"... ", iDestinationPath);
  
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
#ifdef __linux__
  sprintf(buffer, "cp \"%s\" \"%s\" >/dev/null 2>/dev/null", localExec.c_str(), iDestinationPath);
  int return_code = system(buffer);
  sprintf(buffer, "chmod 755 \"%s\" >/dev/null 2>/dev/null", iDestinationPath);
  return_code = system(buffer);
#elif WIN32
  sprintf(buffer, "copy /v \"%s\" \"%s\" >NUL 2>NUL", localExec.c_str(), iDestinationPath);
  int return_code = system(buffer);
#endif

  //print an error message if the command failed.
  if (return_code != 0)
  {
    printf("Failed running command: %s\n", buffer);
  }

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
