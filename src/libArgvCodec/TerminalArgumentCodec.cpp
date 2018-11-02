#include "libargvcodec/TerminalArgumentCodec.h"
#include "rapidassist/strings.h"
#include "rapidassist/process.h"

#include <cstring> //for strncmp()

namespace libargvcodec
{

static std::string gBasicShellCharacters = "&|()<>!";
static std::string gSpecialShellCharacters = "*$`";

size_t countCharacters(const std::string & iStr, char c)
{
  size_t count = 0;

  for(size_t i=0; i<iStr.size(); i++)
  {
    if (iStr[i] == c)
      count++;
  }

  return count;
}

size_t countCharacters(const std::string & iStr, const std::string & iCharacters)
{
  size_t count = 0;

  for(size_t i=0; i<iStr.size(); i++)
  {
    for(size_t j=0; j<iCharacters.size(); j++)
    {
      char c = iCharacters[j];
      if (iStr[i] == c)
        count++;
    }
  }

  return count;
}

TerminalArgumentCodec::TerminalArgumentCodec()
{
}

TerminalArgumentCodec::~TerminalArgumentCodec()
{
}

//IArgumentEncoder
std::string TerminalArgumentCodec::encodeArgument(const char * iValue)
{
  std::string plainArgument;
  if (iValue)
    plainArgument = iValue;

  //Rule 6. Deal with empty argument ASAP
  if (plainArgument == "")
  {
    return std::string("\"\"");
  }

  //prettier optimizations...
  if (plainArgument == "||")
    return std::string("\"||\"");
  if (plainArgument == "&&")
    return std::string("\"&&\"");

  //check flags

  //Rule 2.
  bool isStringArgument = false;
  isStringArgument = isStringArgument || (plainArgument.find(" ") != std::string::npos);
  isStringArgument = isStringArgument || (plainArgument.find("\t") != std::string::npos);

  //Rule 5.1.
  int numBasicShellCharacters = countCharacters(plainArgument, gBasicShellCharacters);
  int numSpecialShellCharacters = countCharacters(plainArgument, gSpecialShellCharacters);
  int numShellCharacters = numBasicShellCharacters + numSpecialShellCharacters;
  if (numBasicShellCharacters >= 2)
  {
    //too many shell characters. Escaping all would requires too many \ characters.
    //it is better to force using a string.
    isStringArgument = true;
  }

  //Rule 5.4
  isStringArgument = isStringArgument || (ra::strings::uppercase(plainArgument) == "TRUE");
  isStringArgument = isStringArgument || (ra::strings::uppercase(plainArgument) == "FALSE");

  //Define string type to know how to encode Rule 3.
  int numDoubleQuotes = countCharacters(plainArgument, '\"');
  int numSingleQuote  = countCharacters(plainArgument, '\'');
  char stringCharacter = '\0';
  if (isStringArgument)
  {
    if (numSingleQuote >= numDoubleQuotes)
      stringCharacter = '\"';
    else
      stringCharacter = '\'';
  }

  std::string escapedArg = iValue;

  //Rule 3.
  if (isStringArgument)
  {
    if (stringCharacter == '\"')
    {
      //relace " by \"
      ra::strings::replace(escapedArg, "\"", "\\\"");

      //Inside a double-quotes string, single-quote characters must be interpreted literally and does not requires escaping.
    }
    else if (stringCharacter == '\'')
    {
      //relace ' by \'
      ra::strings::replace(escapedArg, "'", "\\'");

      //Inside a single-quote string, double-quotes characters must be interpreted literally and does not requires escaping.
    }
  }
  else
  {
    //relace " by \"
    ra::strings::replace(escapedArg, "\"", "\\\"");

    //relace ' by \'
    ra::strings::replace(escapedArg, "'", "\\'");
  }

  //Rule 4.
  //relace \ characters by \\ characters
  ra::strings::replace(escapedArg, "\\", "\\\\");

  //Rule 5.2
  if (!isStringArgument)
  {
    for(size_t i=0; i<gBasicShellCharacters.size(); i++)
    {
      char old_value[2] = {0};
      old_value[0] = gBasicShellCharacters[i];
      char new_value[] = {'\\', '_', '\0'};
      new_value[1] = old_value[0];

      //do the replacement
      ra::strings::replace(escapedArg, old_value, new_value);
    }
  }

  //Rule 5.3.
  {
    for(size_t i=0; i<gSpecialShellCharacters.size(); i++)
    {
      char old_value[2] = {0};
      old_value[0] = gSpecialShellCharacters[i];
      char new_value[] = {'\\', '_', '\0'};
      new_value[1] = old_value[0];

      //do the replacement
      ra::strings::replace(escapedArg, old_value, new_value);
    }
  }

  //Rule 2.
  if (isStringArgument)
  {
    //conver the argument to string
    escapedArg.insert(0, 1, stringCharacter);
    escapedArg.append(1, stringCharacter);
  }

  return escapedArg;
}

std::string TerminalArgumentCodec::encodeCommandLine(const ArgumentList & iArguments)
{
  std::string cmdLine;

  for(int i=1; i<iArguments.getArgc(); i++) //skip first element since it refers to the actual .exe that was launched
  {
    const char * argValue = iArguments.getArgument(i);
    std::string arg = encodeArgument(argValue);
    cmdLine.append(arg);

    //add a space between arguments (except the last one)
    bool isLastArgument = !(i+1<iArguments.getArgc());
    if (!isLastArgument)
      cmdLine.append(" ");
  }

  return cmdLine;
}

//IArgumentDecoder
std::string TerminalArgumentCodec::decodeArgument(const char * iValue)
{
  ArgumentList arglist = decodeCommandLine(iValue);
  if (arglist.getArgc() >= 2) //at least a exe name and an argument
  {
    std::string arg = arglist.getArgument(1);
    return arg;
  }

  return std::string();
}

ArgumentList TerminalArgumentCodec::decodeCommandLine(const char * iValue)
{
  ArgumentList arglist;

  ArgumentList::StringList args;
  bool success = parseCmdLine(iValue, args);
  if (success)
  {
    //insert local .exe path
    args.insert( args.begin(), ra::process::getCurrentProcessPath() );

    arglist.init(args);
  }

  return arglist;
}

bool TerminalArgumentCodec::isArgumentSeparator(const char c)
{
  bool isSeparator = (c == '\0' || c == ' ' || c == '\t');
  return isSeparator;
}

bool TerminalArgumentCodec::isShellCharacter(const char c)
{
  for(size_t i=0; i<gBasicShellCharacters.size(); i++)
  {
    if (c == gBasicShellCharacters[i])
      return true;
  }
  for(size_t i=0; i<gSpecialShellCharacters.size(); i++)
  {
    if (c == gSpecialShellCharacters[i])
      return true;
  }

  return false;
}

bool TerminalArgumentCodec::hasShellCharacters(const char * iValue)
{
  if (iValue == NULL)
    return false;

  size_t len = std::string(iValue).size();
  for(size_t i=0; i<len; i++)
  {
    char c = iValue[i];
    if (isShellCharacter(c))
      return true;
  }
  return false;
}

char TerminalArgumentCodec::getSafeCharacter(const char * iValue, size_t iIndex)
{
  if (iIndex > std::string(iValue).size())
    return '\0';
  return iValue[iIndex];
}

bool TerminalArgumentCodec::parseCmdLine(const char * iCmdLine, ArgumentList::StringList & oArguments)
{
  if (iCmdLine == NULL)
    return false;

  oArguments.clear();

  std::string accumulator;

  const std::string cmdLineStr = iCmdLine;

  bool inDoubleQuotesString = false;
  bool inSingleQuoteString = false;
  bool isValidEmptyArgument = false;
  bool isJuxtaposedString = false;

  for(size_t i=0; i<cmdLineStr.size(); i++)
  {
    char c = iCmdLine[i];

    bool isLastCharacter = !(i+1<cmdLineStr.size());

    if (c == '\"' && !inDoubleQuotesString && !inSingleQuoteString)
    {
      //Rule 2.
      //new double-quotes string
      inDoubleQuotesString = true;
      inSingleQuoteString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = (i == 0);
    }
    else if (c == '\'' && !inDoubleQuotesString && !inSingleQuoteString)
    {
      //Rule 2.
      //new single-quote string
      inDoubleQuotesString = false;
      inSingleQuoteString = true;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = (i == 0);
    }
    else if (c == '\"' && inSingleQuoteString)
    {
      //Rule 3.
      //literal double-quotes
      accumulator.push_back(c);
    }
    else if (c == '\'' && inDoubleQuotesString)
    {
      //Rule 3.
      //literal single-quote
      accumulator.push_back(c);
    }
    else if (c == '\"' && inDoubleQuotesString )
    {
      //Rule 2.
      //ends double-quotes string
      inDoubleQuotesString = false;
      inSingleQuoteString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isLastCharacter;
      if (isValidEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }
    }
    else if (c == '\'' && inSingleQuoteString )
    {
      //Rule 2.
      //ends single-quote string
      inDoubleQuotesString = false;
      inSingleQuoteString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isLastCharacter;
      if (isValidEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }
    }
    else if ( isArgumentSeparator(c) && !inDoubleQuotesString && !inSingleQuoteString )
    {
      //Rule 1.
      //argument separator

      //flush accumulator
      if (accumulator != "")
      {
        oArguments.push_back(accumulator);
        accumulator = "";
      }
    }
    else if (c == '\\')
    {
      //(escaped character).
      //Rule 3.
      //Rule 4.
      //Rule 5.2
      //Rule 5.4

      char next = getSafeCharacter(iCmdLine, i+1);
      if (next != '\0')
      {
        accumulator.push_back(next);
        i++; //skip next character
      }
    }
    else
    {
      //Rule 7.
      //plain text character
      accumulator.push_back(c);
    }

    //next character
  }

  //flush accumulator
  if (accumulator != "")
  {
    oArguments.push_back(accumulator);
    accumulator = "";
  }

  return true;
}

}; //namespace libargvcodec
