#include "TestUtils.h"
#include <cstdio> //for sprintf()
#include <string.h> //for strdup()
#include <cstdlib> //for free()

#include "rapidassist/random.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/process.h"
#include "rapidassist/gtesthelp.h"

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
