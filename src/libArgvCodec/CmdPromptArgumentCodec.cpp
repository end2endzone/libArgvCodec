#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "rapidassist/strings.h"
#include "rapidassist/process.h"

#include <cstring> //for strncmp()

namespace libargvcodec
{

size_t findNumTrailingBackslashes(const char * iValue)
{
  if (iValue == NULL)
    return 0;

  size_t numTrailingBackslashes = 0;
  size_t len = std::string(iValue).size();
  for(size_t i=0; i<len; i++)
  {
    char c = iValue[i];
    if (c == '\\')
      numTrailingBackslashes++;
    else
      numTrailingBackslashes = 0;
  }
  return numTrailingBackslashes;
}

enum CharacterCodes
{
  Plain,
  Delimiter,
  Skipped,
  EscapingBackslash,
  EscapingCaret,
  StringStart,
  StringEnd,
  Rule2,
  Rule3,
  CaretStringStart,
  CaretStringEnd,
  EmptyArgument,
  JuxtaposedStringStart,
  JuxtaposedCaretStringStart,
};

typedef std::vector<CharacterCodes> CodeList;

CmdPromptArgumentCodec::CmdPromptArgumentCodec()
{
}

CmdPromptArgumentCodec::~CmdPromptArgumentCodec()
{
}

//IArgumentEncoder
std::string CmdPromptArgumentCodec::encodeArgument(const char * iValue)
{
  //http://blogs.msdn.com/b/twistylittlepassagesallalike/archive/2011/04/23/everyone-quotes-arguments-the-wrong-way.aspx
  //http://stackoverflow.com/questions/2393384/escape-string-for-process-start
  //http://stackoverflow.com/questions/5510343/escape-command-line-arguments-in-c-sharp/6040946#6040946

  std::string plainArgument = iValue;

  //Rule 6. Deal with empty argument ASAP
  if (plainArgument == "")
  {
    return std::string("\"\"");
  }

  //check flags

  //Rule 1.1.
  bool isStringArgument = false;
  isStringArgument = isStringArgument || (plainArgument.find(" ") != std::string::npos);
  isStringArgument = isStringArgument || (plainArgument.find("\t") != std::string::npos);

  //Rule 5.1.
  bool hasShellCharacters_ = hasShellCharacters(plainArgument.c_str());

  //Optimization...
  //Encode shell characters with a caret characters if argument is not a string and does not contains " or \ characters
  //otherwise, encode using a string
  //ie: test&whoami should be encoded as test^&whoami instead of "test&whoami"
  bool hasDoubleQuotes = (plainArgument.find("\"") != std::string::npos);
  bool hasBackslash    = (plainArgument.find("\\") != std::string::npos);
  isStringArgument = isStringArgument || ( hasShellCharacters_ && (hasDoubleQuotes || hasBackslash) ); 
  
  //Rule 4.
  bool isCaretStringArgument = false; // isStringArgument && hasShellCharacters_;

  std::string escapedArg;
  size_t numBackslashes = 0;
  //for each characters
  for(size_t i=0; i<plainArgument.size(); i++)
  {
    char c = plainArgument[i];

    if (isShellCharacter(c))
    {
      //Rule 3.
      //Unescaped \ characters
      //flush backslashes accumulator
      escapedArg.append( numBackslashes, '\\' );
      numBackslashes = 0;

      //escape character
      if (isStringArgument && !isCaretStringArgument)
      {
        //Rule 5.1.
        //special shell character inside a string which is safe to *NOT* escape
        escapedArg.append(1, c);
      }
      else
      {
        //Rule 5.2.
        //not a string or using a caret-string. Must escape
        escapedArg.append(1, '^');
        escapedArg.append(1, c);
      }
    }
    else if (c == '\"')
    {
      //must always escape
      //choosing to always use \" as escaping method

      //Rule 3.
      //flush backslashes first
      escapedArg.append( (2*numBackslashes), '\\' );
      numBackslashes = 0;

      if (isCaretStringArgument)
      {
        //Rule 2.1.
        escapedArg.append(1, '\\');
        escapedArg.append(1, '^');
        escapedArg.append(1, c);
      }
      else if (isStringArgument)
      {
        //Rule 2.
        //escaped " character (using "" for escaping)
        escapedArg.append(1, c);
        escapedArg.append(1, c);
      }
      else
      {
        //Rule 2.
        //escaped " character (using \" for escaping)
        escapedArg.append(1, '\\');
        escapedArg.append(1, c);
      }
    }
    else if(c == '\\')
    {
      //Rule 3.
      //accumulate
      numBackslashes++;
    }
    else
    {
      //Rule 3.
      //Unescaped \ characters
      //flush backslashes accumulator
      escapedArg.append( numBackslashes, '\\' );
      numBackslashes = 0;

      //Rule 1.
      //plain character
      escapedArg.append( 1, c );
      numBackslashes = 0;
    }
  }

  //Rule 3.
  //Unescaped \ characters
  //flush backslashes accumulator
  escapedArg.append( numBackslashes, '\\' );
  numBackslashes = 0;

  //deal with flags
  if (isCaretStringArgument)
  {
    //Rule 4.
    //wrap escapedArg with ^" starts/ends caret-string commands
    escapedArg.insert(0, "^\"");

    //Rule 3.
    //but watch out for arguments that ends with \ character
    size_t numTrailingBackslashes = findNumTrailingBackslashes(escapedArg.c_str());
    if (numTrailingBackslashes > 0)
    {
      escapedArg.append(numTrailingBackslashes, '\\');
    }

    escapedArg.append("^\"");
  }
  else if (isStringArgument)
  {
    //Rule 1.
    //wrap escapedArg with " starts/ends string commands
    escapedArg.insert(0, "\"");

    //Rule 3.
    //but watch out for arguments that ends with \ character
    size_t numTrailingBackslashes = findNumTrailingBackslashes(escapedArg.c_str());
    if (numTrailingBackslashes > 0)
    {
      escapedArg.append(numTrailingBackslashes, '\\');
    }

    escapedArg.append(1, '\"');
  }
  //else
  // no problem

  return escapedArg;
}

std::string CmdPromptArgumentCodec::encodeCommandLine(const ArgumentList & iArguments)
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
std::string CmdPromptArgumentCodec::decodeArgument(const char * iValue)
{
  ArgumentList arglist = decodeCommandLine(iValue);
  if (arglist.getArgc() >= 2) //at least a exe name and an argument
  {
    std::string arg = arglist.getArgument(1);
    return arg;
  }

  return std::string();
}

ArgumentList CmdPromptArgumentCodec::decodeCommandLine(const char * iValue)
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

bool CmdPromptArgumentCodec::isArgumentSeparator(const char c)
{
  bool isSeparator = (c == '\0' || c == ' ' || c == '\t');
  return isSeparator;
}

bool CmdPromptArgumentCodec::isShellCharacter(const char c)
{
  switch(c)
  {
  case '^':
  case '&':
  case '|':
  case '(':
  case ')':
  case '<':
  case '>':
  case '%':
  case '!':
    return true;
  default:
    return false;
  };
}

bool CmdPromptArgumentCodec::hasShellCharacters(const char * iValue)
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

bool CmdPromptArgumentCodec::supportsShellCharacters()
{
  return true;
}

char CmdPromptArgumentCodec::getSafeCharacter(const char * iValue, size_t iIndex)
{
  if (iIndex > std::string(iValue).size())
    return '\0';
  return iValue[iIndex];
}

bool CmdPromptArgumentCodec::strStart(const char * iStr, const char * iToken)
{
  if (iStr == NULL || iToken == NULL)
    return false;
  bool match = (strncmp(iStr, iToken, strlen(iToken)) == 0);
  return match;
}

bool CmdPromptArgumentCodec::strStart(const char * iStr, size_t iOffset, const char * iToken)
{
  return strStart( &iStr[iOffset], iToken );
}

bool CmdPromptArgumentCodec::matchesBackSlashDblQuoteSequence(const char * iValue, size_t iValueOffset, size_t & oNumBlackSlash, size_t & oSequenceLength, bool iInString, bool iInCaretString)
{
  oNumBlackSlash = 0;
  size_t lastBackSlashOffset = 0;
  //size_t sequenceLength = 0;

  //count the maximum sequence of \ characters
  //the sequence ends with a " character.
  //the sequence can also ends with ^" characters but only if !iInString || iInCaretString and encoder has support for shell characters
  bool acceptCaretCharacters = (supportsShellCharacters() && (iInCaretString || !iInString) );
  char c = iValue[iValueOffset+oSequenceLength];

  while( c == '\\' || (acceptCaretCharacters && /*c == '^'*/ strStart(iValue, iValueOffset+oSequenceLength, "^\\")) ) //allow accepting sequences in the following format:    ^"a\^\^\\"b"
  {
    if (c != '\\')
    {
      //assume its a ^\ character sequence

      //skip ^ character
      oSequenceLength++;
    }
    oNumBlackSlash++;
    lastBackSlashOffset = iValueOffset+oSequenceLength;

    oSequenceLength++; //next character
    c = iValue[iValueOffset+oSequenceLength];
  }

  bool validString      = (oNumBlackSlash > 0 && iValue[iValueOffset+oSequenceLength] == '\"');
  bool validCaretString = (oNumBlackSlash > 0 && supportsShellCharacters() && (iInCaretString || !iInString) && strStart(&iValue[iValueOffset+oSequenceLength], "^\"") );
  bool valid = validString || validCaretString;

  //Compute skip offset
  if (valid)
  {
    if (oNumBlackSlash%2 == 0)
    {
      //the next processed character must be " (or ^")
      //oSequenceLength stays the same
    }
    else
    {
      //the next processed character must be the last \ character of the sequence
      //ie: a\\\"b   =>    a\"b
      oSequenceLength--;
    }
  }
  else
  {
    oNumBlackSlash = 0;
    oSequenceLength = 0;
  }

  return valid;
}

bool CmdPromptArgumentCodec::isStringStart(const char * iCmdLine, size_t iOffset)
{
  //Validate Rule 6.
  if (iOffset == 0)
    return true;

  //find previous character that is not a ^
  size_t offset = iOffset-1;
  char previous = getSafeCharacter(iCmdLine, offset);
  while (previous == '^')
  {
    offset--;
    previous = getSafeCharacter(iCmdLine, offset);
  }
  return isArgumentSeparator(previous);
}

bool CmdPromptArgumentCodec::isStringEnd(const char * iCmdLine, size_t iOffset, size_t iSequenceLength)
{
  //Validate Rule 6.
  if (iCmdLine[iOffset+iSequenceLength] == '\0')
    return true;

  //find next character that is not a ^
  size_t offset = iOffset+iSequenceLength;
  char previous = getSafeCharacter(iCmdLine, offset);
  while (previous == '^')
  {
    offset--;
    previous = getSafeCharacter(iCmdLine, offset);
  }
  return isArgumentSeparator(previous);
}

bool CmdPromptArgumentCodec::parseCmdLine(const char * iCmdLine, ArgumentList::StringList & oArguments)
{
  CodeList codes;

  oArguments.clear();

  std::string accumulator;

  const std::string cmdLineStr = iCmdLine;

  bool inString = false;
  bool inCaretString = false;
  bool isValidEmptyArgument = false;
  bool isJuxtaposedString = false;

  for(size_t i=0; i<cmdLineStr.size(); i++)
  {
    char c = iCmdLine[i];

    bool isLastCharacter = !(i+1<cmdLineStr.size());
    size_t numBackSlashes = 0;
    size_t backslashSequenceLength = 0;

    if ( supportsShellCharacters() && !inString && !inCaretString && strStart(iCmdLine, i, "^\""))
    {
      //Rule 4.
      //new caret-string
      inString = false;
      inCaretString = true;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isStringStart(iCmdLine, i);
      
      //Rule 7.
      bool isJuxtaposedString = 
        (
          codes.size() > 0 &&
          (codes[codes.size()-1] == StringEnd || codes[codes.size()-1] == CaretStringEnd )
        );
      if (isJuxtaposedString)
      {
        accumulator.push_back('\"');
      }

      //Remember what was found
      if (isJuxtaposedString)
      {
        codes.push_back(JuxtaposedCaretStringStart);
        codes.push_back(JuxtaposedCaretStringStart);
      }
      else
      {
        codes.push_back(CaretStringStart);
        codes.push_back(CaretStringStart);
      }

      i=i+1; //skip next character
    }
    else if ( supportsShellCharacters() && !inString && inCaretString && strStart(iCmdLine, i, "^\""))
    {
      //Rule 4.
      //end caret-string
      inString = false;
      inCaretString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isStringEnd(iCmdLine, i, 2);
      if (isValidEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }

      //Remember what was found
      codes.push_back(CaretStringEnd);
      codes.push_back(CaretStringEnd);

      i=i+1; //skip next character
    }
    else if (c == '^' && inString && !inCaretString)
    {
      //Rule 5.1. Shell character as plain text
      accumulator.push_back(c);

      //Remember what was found
      codes.push_back(Plain);
    }
    else if (supportsShellCharacters() && c == '^' && (!inString || !inCaretString) && strStart(iCmdLine, i, "^^") )
    {
      //Rule 5.2.
      accumulator.push_back(c);

      //Remember what was found
      codes.push_back(EscapingCaret);
      codes.push_back(Plain);

      i=i+1; //skip next character
    }
    else if (supportsShellCharacters() && c == '^' && (!inString || !inCaretString) )
    {
      //Rule 5.2 & 5.3.
      //skip this character

      //Remember what was found
      codes.push_back(EscapingCaret);
    }
    else if (c == '\"' && !inString && !inCaretString)
    {
      //Rule 1.1.
      //new string
      inString = true;
      inCaretString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isStringStart(iCmdLine, i);
      
      //Rule 7.
      bool isJuxtaposedString = 
        (
          codes.size() > 0 &&
          (codes[codes.size()-1] == StringEnd || codes[codes.size()-1] == CaretStringEnd )
        );
      if (isJuxtaposedString)
      {
        accumulator.push_back('\"');
      }

      //Remember what was found
      if (isJuxtaposedString)
      {
        codes.push_back(JuxtaposedStringStart);
        codes.push_back(JuxtaposedStringStart);
      }
      else
      {
        codes.push_back(StringStart);
        codes.push_back(StringStart);
      }
    }
    else if ( inCaretString && !inString && strStart(iCmdLine, i, "\\\"") )
    {
      //Rule 9.1 (exception)
      //   \" sequence in caret-string should be read as [close caret-string] and [open string] and plain " character.

      //end caret-string
      inString = false;
      inCaretString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = false;

      //do not skip " character, which must be interpreted as a string open

      //Remember what was found
      codes.push_back(CaretStringEnd);
    }
    else if ( supportsShellCharacters() && (inCaretString || !inString) && strStart(iCmdLine, i, "\\^\"") )
    {
      //Rule 2.1.
      // for \^" character sequence inside a caret-string or outside a string
      accumulator.push_back('\"');

      //Remember what was found
      codes.push_back(EscapingBackslash);
      codes.push_back(Skipped);
      codes.push_back(Plain);

      i=i+2; //skip next character
    }
    else if ( strStart(iCmdLine, i, "\\\"") )
    {
      //Rule 2.
      // for \" character sequence outside/inside a string or caret-string
      accumulator.push_back('\"');

      //Remember what was found
      codes.push_back(EscapingBackslash);
      codes.push_back(Plain);

      i=i+1; //skip next character
    }
    else if ( matchesBackSlashDblQuoteSequence(iCmdLine, i, numBackSlashes, backslashSequenceLength, inString, inCaretString) )
    {
      //Rule 3.
      // for \\" character sequence (or any combination like \\\" or \\\\" or \\^" or \\\^" or \\\\^" or even \^\^\\" )
      size_t numEscapedBackSlashes = numBackSlashes/2;
      std::string s;
      s.append(numEscapedBackSlashes, '\\');

      accumulator.append(s);

      //Remember what was found
      for(size_t j=0; j<backslashSequenceLength; j++)
      {
        codes.push_back(Skipped);
      }

      i=i+backslashSequenceLength-1; //skip escaped \ characters (but not the last \ if odd backslashes are found) but not the " character
    }
    else if ( (inString || inCaretString) && strStart(iCmdLine, i, "\"\"") )
    {
      //Rule 2.
      // for "" character sequence inside a string or caret-string
      accumulator.push_back('\"');

      //Remember what was found
      codes.push_back(Skipped);
      codes.push_back(Plain);

      i=i+1; //skip next character
    }
    else if (c == '\"' && (inString || inCaretString) )
    {
      //Rule 1.1.
      //ends a new string or caret-string
      inString = false;
      inCaretString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isStringEnd(iCmdLine, i, 1);
      if (isValidEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }

      //Remember what was found
      codes.push_back(StringEnd);
    }
    else if ( isArgumentSeparator(c) && !inString && !inCaretString )
    {
      //Rule 1.
      //argument separator

      //flush accumulator
      if (accumulator != "")
      {
        oArguments.push_back(accumulator);
        accumulator = "";
      }

      //Remember what was found
      codes.push_back(Delimiter);
    }
    else if (c == '\\' && !inString && !inCaretString)
    {
      //Rule 3 (un-escaped).
      accumulator.push_back(c);

      //Remember what was found
      codes.push_back(Plain);
    }
    else
    {
      //Rule 8.
      //plain text character
      accumulator.push_back(c);

      //Remember what was found
      codes.push_back(Plain);
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
