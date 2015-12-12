#include "ArgumentLister.h"
//#include "ArgumentManager.h"
#include "utils.h"

std::string getExePath()
{
  ArgumentManager tmp;
  tmp.init("");
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
  utils::strReplace(printerExec, ".exe", ".printer.exe");
  return printerExec;
}

std::string getLoggerExecFilePath()
{
  std::string localExec = getExePath();

  if (isLoggerExec())
    return localExec;

  std::string loggerExec = localExec;
  utils::strReplace(loggerExec, ".exe", ".logger.exe");
  return loggerExec;
}

std::string getLogFilePath()
{
  std::string logPath = getLoggerExecFilePath();
  utils::strReplace(logPath, ".exe", ".log");
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
    printf("argv[%d]=%s\n", i, argv[i]);
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

std::string decodeArgument(const char * iValue)
{
  ArgumentManager::StringList arguments;
  bool success = decodeCommandLineArguments(iValue, arguments);
  if (arguments.size() != 2)
    return "";

  std::string confirmedArgumentValue = arguments[1];
  return confirmedArgumentValue;
}

bool decodeCommandLineArguments(const char * iCmdLine, ArgumentManager::StringList & oArguments)
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
    utils::strReplace(line, buffer, "");

    //remove ending CRLF at the end of the line;
    utils::strReplace(line, "\n", "");

    oArguments.push_back(line);

    argNumber++;
  }

  fclose(f);
  return true;
}
