#include "libargvcodec/TerminalArgumentCodec.h"
#include "rapidassist/strings.h"
#include "rapidassist/process.h"

#include <cstring> //for strncmp()

namespace libargvcodec
{

static std::string gBasicShellCharacters = "&|()<>*";
static std::string gSpecialShellCharacters = "$`";

bool isSpecialShellCharacter(const char c)
{
  for(size_t i=0; i<gSpecialShellCharacters.size(); i++)
  {
    if (c == gSpecialShellCharacters[i])
      return true;
  }

  return false;
}

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
  static const std::string EMPTY_ARGUMENT = "\"\"";

  //Rule 6.1 Deal with empty argument ASAP
  if (iValue == NULL)
    return EMPTY_ARGUMENT;

  std::string plainArgument = iValue;

  //Rule 6.1 Deal with empty argument ASAP
  if (plainArgument == "")
    return EMPTY_ARGUMENT;

  //prettier optimizations...
  if (plainArgument == "||")
    return std::string("\"||\"");
  if (plainArgument == "&&")
    return std::string("\"&&\"");

  //check flags

  //Rule 2.1
  bool isStringArgument = false;
  isStringArgument = isStringArgument || (plainArgument.find(" ") != std::string::npos);
  isStringArgument = isStringArgument || (plainArgument.find("\t") != std::string::npos);

  //Force a string if too many shell characters.
  //Each shell characters would requires escaping (Rule 5.2) but inside a string,
  //they do not require escaping (Rule 5.1).
  int numBasicShellCharacters = countCharacters(plainArgument, gBasicShellCharacters);
  //int numSpecialShellCharacters = countCharacters(plainArgument, gSpecialShellCharacters);
  //int numShellCharacters = numBasicShellCharacters + numSpecialShellCharacters;
  if (numBasicShellCharacters >= 2)
  {
    //better to force using a string.
    isStringArgument = true;
  }

  //Define string type to know how to encode Rule 3.*.
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

  //Rule 3.*
  if (isStringArgument)
  {
    if (stringCharacter == '\"')
    {
      //Rule 3.4: Double-quote  characters inside a double-quotes string must be escaped with `\` to be properly interpreted.
      //relace " by \"
      ra::strings::replace(escapedArg, "\"", "\\\"");

      //Rule 3.2: Single-quote  characters inside a double-quotes string must be interpreted literally and does not requires escaping.
      //nothing to do
    }
    else if (stringCharacter == '\'')
    {
      //Rule 3.5: Single-quote  characters inside a single-quote  string **CAN NOT** be escaped with `\`. The single-quote string must be ended, joined with an escaped single-quote and reopened to be properly interpreted.
      //relace ' by '\''
      ra::strings::replace(escapedArg, "'", "'\\''");

      //Rule 3.3: Double-quotes characters inside a single-quote  string must be interpreted literally and does not requires escaping.
      //nothing to do
    }
  }
  else
  {
    //Rule 3.1: Literal `'` or `"` characters must be escaped with `\` when outside a string. The character does not starts/ends a string.
    
    //relace " by \"
    ra::strings::replace(escapedArg, "\"", "\\\"");

    //relace ' by \'
    ra::strings::replace(escapedArg, "'", "\\'");
  }

  //Rule 4.*
  if (!isStringArgument)
  {
    //Rule 4.1: The character `\` must be escaped with `\` (resulting in `\\`) when outside a string.
    //relace \ characters by \\ characters
    ra::strings::replace(escapedArg, "\\", "\\\\");
  }
  else if (isStringArgument && stringCharacter == '\"')
  {
    //Rule 4.4. Two consecutive `\` characters in a double-quotes string must be interpreted as a literal `\` character.
    //relace \ characters by \\ characters
    ra::strings::replace(escapedArg, "\\", "\\\\");
  }
  else if (isStringArgument && stringCharacter == '\'')
  {
    //Rule 4.5. The character `\` does not requires escaping when inside a single-quote string.
    //nothing to do
  }

  //Rule 5.2: The shell characters `&`,`|`,`(`,`)`,`<`,`>` or `*` must be escapsed with `\` when outside a string.
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

  //Rule 5.3: The shell characters `$`, and `` ` `` (backtick) are special shell characters and must *always* be escapsed with `\`.
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

  //Rule 2.1. If an argument contains [space] or tab characters, it must be enclosed in a string to form a single argument.
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
  bool isJuxtaposedString = false;
  size_t stringStartOffset = std::string::npos;

  for(size_t i=0; i<cmdLineStr.size(); i++)
  {
    const char c = iCmdLine[i];
    const char next = getSafeCharacter(iCmdLine, i+1);

    bool isLastCharacter = !(i+1<cmdLineStr.size());

    if (c == '\"' && !inDoubleQuotesString && !inSingleQuoteString)
    {
      //Rule 2.2. Double quotes character `"` starts/ends a string. The `"` character is omitted from the argument.
      //new double-quotes string
      inDoubleQuotesString = true;
      inSingleQuoteString = false;

      //Rule 6.1: Empty arguments must be specified with `""` and must be surrounded by argument delimiters or located at the start/or end of the command line.
      //Rule 6.2: Empty arguments can also be specified with `''`.
      stringStartOffset = i;
    }
    else if (c == '\'' && !inDoubleQuotesString && !inSingleQuoteString)
    {
      //Rule 2.3: Single quote character `'` also starts/ends a string. The `'` character is omitted from the argument.
      //new single-quote string
      inDoubleQuotesString = false;
      inSingleQuoteString = true;

      //Rule 6.1: Empty arguments must be specified with `""` and must be surrounded by argument delimiters or located at the start/or end of the command line.
      //Rule 6.2: Empty arguments can also be specified with `''`.
      stringStartOffset = i;
    }
    else if (c == '\"' && inSingleQuoteString)
    {
      //Rule 3.3: Double-quotes characters inside a single-quote  string must be interpreted literally and does not requires escaping.
      //literal double-quotes
      accumulator.push_back(c);
    }
    else if (c == '\'' && inDoubleQuotesString)
    {
      //Rule 3.2: Single-quote  characters inside a double-quotes string must be interpreted literally and does not requires escaping.
      //literal single-quote
      accumulator.push_back(c);
    }
    else if (c == '\"' && inDoubleQuotesString)
    {
      //Rule 2.2: Double quotes character `"` starts/ends a string. The `"` character is omitted from the argument.
      //ends double-quotes string
      inDoubleQuotesString = false;
      inSingleQuoteString = false;

      //Rule 6.1: Empty arguments must be specified with `""` and must be surrounded by argument delimiters or located at the start/or end of the command line.
      //Rule 6.2: Empty arguments can also be specified with `''`.
      bool isEmptyString = (i == stringStartOffset+1);
      bool isEmptyArgument = (isEmptyString && accumulator.empty());
      if (isEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }
    }
    else if (c == '\'' && inSingleQuoteString)
    {
      //Rule 2.3: Single quote character `'` also starts/ends a string. The `'` character is omitted from the argument.
      //ends single-quote string
      inDoubleQuotesString = false;
      inSingleQuoteString = false;

      //Rule 6.1: Empty arguments must be specified with `""` and must be surrounded by argument delimiters or located at the start/or end of the command line.
      //Rule 6.2: Empty arguments can also be specified with `''`.
      bool isEmptyString = (i == stringStartOffset+1);
      bool isEmptyArgument = (isEmptyString && accumulator.empty());
      if (isEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }
    }
    else if (isArgumentSeparator(c) && !inDoubleQuotesString && !inSingleQuoteString)
    {
      //Rule 1.1: [space] or tab characters are argument delimiters/separators but *ONLY* when outside a string.
      //argument separator

      //flush accumulator
      if (accumulator != "")
      {
        oArguments.push_back(accumulator);
        accumulator = "";
      }
    }
    else if (c == '\\' && !inDoubleQuotesString && !inSingleQuoteString)
    {
      //Rule 4.1. The character `\` must be escaped with `\` (resulting in `\\`) when outside a string.
      //Rule 4.2: Characters escaped with `\` are literal characters.
      //Rule 5.2: The shell characters `&`,`|`,`(`,`)`,`<`,`>` or `*` must be escapsed with `\` when outside a string.
      //Rule 5.3: The shell characters `$`, and `` ` `` (backtick) are special shell characters and must *always* be escapsed with `\`.

      if (next != '\0')
      {
        accumulator.push_back(next);
        i++; //skip next character
      }
    }
    else if (c == '\\' && inDoubleQuotesString && next == '\"')
    {
      //Rule 3.4: Double quote  characters inside a double quotes string must be escaped with `\` to be properly interpreted.

      accumulator.push_back(next);
      i++; //skip next character
    }
    else if (c == '\\' && inDoubleQuotesString && next == '\\')
    {
      //Rule 4.4: Two consecutive `\` characters in a double-quotes string must be interpreted as a literal `\` character.

      accumulator.push_back(c);
      i++; //skip next character
    }
    else if (c == '\\' && isSpecialShellCharacter(next))
    {
      //Rule 5.3: The shell characters `$`, and `` ` `` (backtick) are special shell characters and must *always* be escapsed with `\`.

      accumulator.push_back(next);
      i++; //skip next character
    }
    else if (c == '\\' && inSingleQuoteString)
    {
      //Rule 4.5: The character `\` does not requires escaping when inside a single-quote string.
      accumulator.push_back(c);
    }
    else
    {
      //Rule 7: All other characters must be read as plain text.
      
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
