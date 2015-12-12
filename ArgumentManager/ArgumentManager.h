#pragma once

#include "ArgMgrExports.h"
#include <string>
#include <vector>

class ARGMGR_API ArgumentManager
{
public:
  typedef std::vector<std::string> StringList;

  ArgumentManager();
  ArgumentManager(const ArgumentManager & iArgumentManager);
  virtual ~ArgumentManager();

  void init(int argc, char** argv);
  void init(const char * iCmdLine);
  void init(const char * iCmdLine, bool iIncludeExec);
  void init(const StringList & iArguments);

  char * getArgument(int iIndex);
  std::string getCommandLine();
  std::string getCommandLineArgument(int iIndex);

  int getArgc();
  char** getArgv();

  //logical methods
  const ArgumentManager & operator = (const ArgumentManager & iArgumentManager);
  bool operator == (const ArgumentManager & iArgumentManager) const;
  bool operator != (const ArgumentManager & iArgumentManager) const;

  //manipulation methods
  bool insert(int iIndex, const char * iValue);
  bool insert(const char * iValue);
  bool remove(int iIndex);
  bool replace(int iIndex, const char * iValue);

  //parsing methods

  //
  // Description:
  //  Finds the exact iValue within all parameters.
  // Return:
  //  Returns the index of the parameter if found. Returns -1 otherwise.
  //
  int findIndex(const char * iValue);

  //
  // Description:
  //  Tells if the exact iValue can be found within all parameters.
  // Return:
  //  Returns the true if iValue can be found. Returns false otherwise.
  //
  bool contains(const char * iValue);

  //
  // Description:
  //  Tells if the exact option iValue is specified as a parameter.
  // Parameters:
  //  iValue: The option we are looking for including any option prefix like the following characters: /, - or --
  //  oIndex: The index where the option was found. Set to -1 if option if not found.
  // Return:
  //  Returns the true if iValue is specified. Returns false otherwise.
  //
  bool findOption(const char * iValue);
  bool findOption(const char * iValue, int & oIndex);
  
  //
  // Description:
  //  Tells if the exact argument named value iValue is specified as a parameter.
  //  Named values are specified with the following format: [prefix]name=value (where prefix is optional)
  // Parameters:
  //  iValueName: The option's name are looking for.
  //              Must include any prefix like the following characters: "-name=", "--name="
  //              Must include the equal sign.
  //  oIndex: The index where the option was found. Set to -1 if option if not found.
  //  oValue: The value of the option name. Set to empty string (or 0) if not found.
  // Return:
  //  Returns the true if the option's name is specified. Returns false otherwise.
  //
  bool findValue(const char * iValueName, int & oIndex, std::string & oValue);
  bool findValue(const char * iValueName, int & oIndex, int & oValue);

  //
  // Description:
  //  Same as: bool findOption(char* iValue);
  //  Removes the option from the argument list if found.
  //
  bool extractOption(const char * iValue);

  //
  // Description:
  //  Same as: bool findValue(char* iValueName, std::string & oValue);
  //  Removes the option from the argument list if found.
  //
  bool extractValue(const char * iValueName, std::string & oValue);
  bool extractValue(const char * iValueName, int & oValue);

  //
  // Description:
  //  Extracts a name=value pair from the list of parametes.
  //  Removes the value from the argument list if found.
  // Parameters:
  //  iValueName: The option's name are looking for.
  //              Must include any prefix like the following characters: "-name=", "--name="
  //              Must include the equal sign.
  // Returns:
  //  Returns the option's value if found. Returns an empty string otherwise
  //
  std::string extractValue(const char * iValueName);

private:

  void rebuildArgv();
  char** mArgv;
  bool isValid(int iIndex);
  bool parseCmdLine(const char * iCmdLine, StringList & oArguments);

  StringList mArguments;
};
