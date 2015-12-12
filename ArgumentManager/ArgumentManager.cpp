// ArgMgr.cpp : Defines the exported functions for the DLL application.
//

#include "targetver.h"

#include "ArgumentManager.h"
#include "utils.h"
#include "os.h"
#include <assert.h>
#include <vector>

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

std::string ArgumentManager::getCommandLine()
{
  std::string cmdLine;

  for(size_t i=1; i<mArguments.size(); i++) //skip first element since it refers to the actual .exe that was launched
  {
    std::string arg = getCommandLineArgument((int)i);
    cmdLine.append(arg);

    //add a space between arguments (except the last one)
    bool isLastArgument = !(i+1<mArguments.size());
    if (!isLastArgument)
      cmdLine.append(" ");
  }

  return cmdLine;
}

std::string ArgumentManager::getCommandLineArgument(int iIndex)
{
  //http://blogs.msdn.com/b/twistylittlepassagesallalike/archive/2011/04/23/everyone-quotes-arguments-the-wrong-way.aspx
  //http://stackoverflow.com/questions/2393384/escape-string-for-process-start
  //http://stackoverflow.com/questions/5510343/escape-command-line-arguments-in-c-sharp/6040946#6040946


  //NOT IMPLEMENTED
  //TODO: IMPLEMENT ArgumentManager::getCommandLine()

  std::string plainArgument = getArgument(iIndex);

  //Rule 10. Deal with empty argument ASAP
  if (plainArgument == "")
  {
    return std::string("\"\"");
  }

  //check flags

  //Rule 9.
  bool isStringArgument = false;
  isStringArgument = isStringArgument || (plainArgument.find(" ") != std::string::npos);
  isStringArgument = isStringArgument || (plainArgument.find("\t") != std::string::npos);

  bool hasShellCharacters_ = hasShellCharacters(plainArgument.c_str());
  bool isCaretStringArgument = isStringArgument && hasShellCharacters_;
  
  //Unknown rule.
  //force caret-string if a shell character follows a " character.
  for(size_t i=0; i<plainArgument.size(); i++)
  {
    char c = plainArgument[i];
    bool isLastChar = !((i+1)<plainArgument.size());
    if (!isLastChar && c == '\"' && isShellCharacter(plainArgument[i+1]))
      isCaretStringArgument = true;
  }

  std::string escapedArg;
  size_t numBackslashes = 0;
  //for each characters
  for(size_t i=0; i<plainArgument.size(); i++)
  {
    char c = plainArgument[i];

    if (isShellCharacter(c))
    {
      //flush backslashes accumulator
      if (numBackslashes > 0)
      {
        escapedArg.append(numBackslashes, '\\');
      }

      //escape character
      if (isStringArgument && !isCaretStringArgument)
      {
        //Rule 5.
        //special shell character inside a string which is safe to *NOT* escape
        escapedArg.append(1, c);
      }
      else
      {
        //Rule 8.
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
        //Rule 7.
        escapedArg.append(1, '\\');
        escapedArg.append(1, '^');
        escapedArg.append(1, c);
      }
      else
      {
        //Rule 2.
        escapedArg.append(1, '\\');
        escapedArg.append(1, c);
      }
    }
    else if(c == '\\')
    {
      //accumulate
      numBackslashes++;
    }
    else
    {
      //flush backslashes accumulator
      if (numBackslashes > 0)
      {
        escapedArg.append(numBackslashes, '\\');
      }

      //Rule 11.
      //plain character
      escapedArg.append( 1, c );
      numBackslashes = 0;
    }
  }

  //flush backslashes accumulator
  if (numBackslashes > 0)
  {
    escapedArg.append(numBackslashes, '\\');
  }

  //deal with flags
  if (isCaretStringArgument)
  {
    //Rule 6.
    //wrap escapedArg with ^" starts/ends caret-string commands
    escapedArg.insert(0, "^\"");

    //Rule 3.
    //but watch out for arguments that ends with \ character
    size_t numTrailingBackslashes = utils::findNumTrailingBackslashes(escapedArg.c_str());
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
    size_t numTrailingBackslashes = utils::findNumTrailingBackslashes(escapedArg.c_str());
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
  //the last element of argv must be an empty string (NULL character)
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

char getSafeCharacter(const char * iValue, size_t iIndex)
{
  if (iIndex > std::string(iValue).size())
    return '\0';
  return iValue[iIndex];
}

bool matchesSequence(const char * iValue, const char * iSequenceExpr)
{
  size_t seqLen   = std::string(iSequenceExpr).size();
  size_t valueLen = std::string(iValue).size();

  if (valueLen < seqLen)
  {
    //value smaller than sequence,
    //unable to find sequence in value
    return false;
  }
  bool match = (strncmp(iValue, iSequenceExpr, seqLen) == 0);
  return match;
}
bool matchesSequence(const char * iValue, size_t iValueOffset, const char * iSequenceExpr)
{
  return matchesSequence( &iValue[iValueOffset], iSequenceExpr );
}
bool matchesBackSlashDblQuoteSequence(const char * iValue, size_t iValueOffset, size_t & oNumBlackSlash, size_t & oSkipLength, bool iInString, bool iInCaretString)
{
  oNumBlackSlash = 0;
  size_t quoteOffset = 0;
  size_t lastBackSlashOffset = 0;

  //count the maximum sequence of \ characters
  char c = iValue[iValueOffset+quoteOffset];
  while( c == '\\' || (iInCaretString && c == '^') )
  {
    if (c == '\\')
    {
      oNumBlackSlash++;
      lastBackSlashOffset = quoteOffset;
    }

    quoteOffset++; //next character
    c = iValue[iValueOffset+quoteOffset];
  }

  bool valid = (oNumBlackSlash > 0 && iValue[iValueOffset+quoteOffset] == '\"');

  //Compute skip offset
  if (valid)
  {
    if (oNumBlackSlash%2 == 0)
    {
      //the " character is a starts/ends string character and must not part of the \ sequence.
      //ie: a\\\\"b   =>    a\\[openstring]b
      oSkipLength = quoteOffset;
    }
    else
    {
      //the last " character is an escaped " character and must not be included in the sequence.
      //ie: a\\\"b   =>    a\"b
      oSkipLength = lastBackSlashOffset;
    }
  }
  else
  {
    oSkipLength = 0;
  }

  return valid;
}
bool isStringStart(const char * iCmdLine, size_t iOffset, const CodeList & iCodes)
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
  return ArgumentManager::isArgumentSeparator(previous);
}
bool isStringEnd(const char * iCmdLine, size_t iOffset, size_t iSequenceLength, const CodeList & iCodes)
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
  return ArgumentManager::isArgumentSeparator(previous);
}


bool ArgumentManager::isArgumentSeparator(const char c)
{
  bool isSeparator = (c == '\0' || c == ' ' || c == '\t');
  return isSeparator;
}

bool ArgumentManager::isShellCharacter(const char c)
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

bool ArgumentManager::hasShellCharacters(const char * iValue)
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

bool ArgumentManager::parseCmdLine(const char * iCmdLine, StringList & oArguments)
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
    size_t backSlashSequenceLength = 0;

    if ( !inString && !inCaretString && matchesSequence(iCmdLine, i, "^\""))
    {
      //Rule 4.
      //new caret-string
      inString = false;
      inCaretString = true;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isStringStart(iCmdLine, i, codes);
      
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
    else if ( !inString && inCaretString && matchesSequence(iCmdLine, i, "^\""))
    {
      //Rule 4.
      //end caret-string
      inString = false;
      inCaretString = false;

      //Rule 6. Validate isEmptyArgumentString
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isStringEnd(iCmdLine, i, 2, codes);
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
    else if (c == '^' && (!inString || !inCaretString) )
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
      isValidEmptyArgument = isStringStart(iCmdLine, i, codes);
      
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
    else if ( inCaretString && !inString && matchesSequence(iCmdLine, i, "\\\"") )
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
    else if ( (inCaretString || !inString) && matchesSequence(iCmdLine, i, "\\^\"") )
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
    else if ( matchesSequence(iCmdLine, i, "\\\"") )
    {
      //Rule 2.
      // for \" character sequence outside/inside a string or caret-string
      accumulator.push_back('\"');

      //Remember what was found
      codes.push_back(EscapingBackslash);
      codes.push_back(Plain);

      i=i+1; //skip next character
    }
    else if ( matchesBackSlashDblQuoteSequence(iCmdLine, i, numBackSlashes, backSlashSequenceLength, inString, inCaretString) )
    {
      //Rule 3.
      // for \\" character sequence (or any combination like \\\" or \\\\" or \\\^" or \\\\^" )
      size_t numEscapedBackSlashes = numBackSlashes/2;
      std::string s;
      s.append(numEscapedBackSlashes, '\\');

      accumulator.append(s);

      //Remember what was found
      for(size_t j=0; j<numEscapedBackSlashes; j++)
      {
        codes.push_back(EscapingBackslash);
        codes.push_back(Plain);
      }

      i=i+backSlashSequenceLength-1; //skip escaped \ characters (but not the last \ if odd backslashes are found) but not the " character
    }
    else if ( (inString || inCaretString) && matchesSequence(iCmdLine, i, "\"\"") )
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
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isStringEnd(iCmdLine, i, 1, codes);
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


    //if ( !inString && !inCaretString && matchesSequence(iCmdLine, i, "^\""))
    //{
    //  //Rule 6 (old).
    //  //new caret-string
    //  inString = false;
    //  inCaretString = true;

    //  //Rule 10 (old). Validate isEmptyArgumentString
    //  char previous = '\0';
    //  {
    //    //find previous character that is not a ^
    //    size_t offset = i-1;
    //    previous = getSafeCharacter(iCmdLine, offset);
    //    while (previous == '^')
    //    {
    //      offset--;
    //      previous = getSafeCharacter(iCmdLine, offset);
    //    }
    //  }
    //  isValidEmptyArgument = isArgumentSeparator(previous);

    //  i=i+1; //skip next character
    //}
    //else if ( !inString && inCaretString && matchesSequence(iCmdLine, i, "^\""))
    //{
    //  //Rule 6 (old).
    //  //end caret-string
    //  inString = false;
    //  inCaretString = false;

    //  //Rule 10 (old). Validate isEmptyArgumentString
    //  char next = '\0';
    //  {
    //    //find next character that is not a ^
    //    size_t offset = i+2;
    //    next = getSafeCharacter(iCmdLine, offset);
    //    while( next == '^' )
    //    {
    //      offset++;
    //      next = getSafeCharacter(iCmdLine, offset);
    //    }
    //    next = getSafeCharacter(iCmdLine, offset);
    //  }
    //  isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isArgumentSeparator(next);

    //  if (isValidEmptyArgument)
    //  {
    //    //insert an empty argument
    //    oArguments.push_back("");
    //  }

    //  i=i+1; //skip next character
    //}
    //else if (c == '^' && inString && !inCaretString)
    //{
    //  //Rule 5 (old).
    //  accumulator.push_back(c);
    //}
    //else if (c == '^' && (!inString || !inCaretString) )
    //{
    //  //Rule 8 (old).
    //  //skip this character
    //}
    //else if (c == '\"' && !inString && !inCaretString)
    //{
    //  //Rule 1 (old).
    //  //new string
    //  inString = true;
    //  inCaretString = false;

    //  //Rule 10 (old). Validate isEmptyArgumentString
    //  char previous = '\0';
    //  {
    //    //find previous character that is not a ^
    //    size_t offset = i-1;
    //    previous = getSafeCharacter(iCmdLine, offset);
    //    while (previous == '^')
    //    {
    //      offset--;
    //      previous = getSafeCharacter(iCmdLine, offset);
    //    }
    //  }
    //  isValidEmptyArgument = isArgumentSeparator(previous);

    //  //ie:
    //  //  a"b  c" d
    //  //  a "b   c"d
    //}
    //else if ( matchesSequence(iCmdLine, i, "\\^\"") && (inCaretString || !inString) )
    //{
    //  //Rule 2 (old).
    //  // for \^" character sequence inside a caret-string
    //  accumulator.push_back('\"');
    //  i=i+2; //skip next character
    //}
    //else if ( matchesSequence(iCmdLine, i, "\\\"") )
    //{
    //  //Rule 2 (old).
    //  // for \" character sequence outside/inside a string or caret-string
    //  accumulator.push_back('\"');
    //  i=i+1; //skip next character
    //}
    //else if ( matchesBackSlashDblQuoteSequence(iCmdLine, i, numBackSlashes, backSlashSequenceLength, inString, inCaretString) )
    //{
    //  //Rule 3 (old).
    //  // for \\" character sequence (or any combination like \\\" or \\\\" )
    //  size_t numEscapedBackSlashes = numBackSlashes/2;
    //  std::string s;
    //  s.append(numEscapedBackSlashes, '\\');

    //  accumulator.append(s);

    //  i=i+backSlashSequenceLength-1; //skip escaped \ characters (but not the last \ if odd backslashes are found) but not the " character
    //}
    //else if ( (inString || inCaretString) && matchesSequence(iCmdLine, i, "\"\"") )
    //{
    //  //Rule 2 (old).
    //  // for "" character sequence inside a string or caret-string
    //  accumulator.push_back('\"');
    //  i=i+1; //skip next character
    //}
    //else if (c == '\"' && (inString || inCaretString) )
    //{
    //  //Rule 1 (old).
    //  //ends a new string or caret-string
    //  inString = false;
    //  inCaretString = false;

    //  //Rule 10 (old). Validate isEmptyArgumentString
    //  char next = '\0';
    //  {
    //    //find next character that is not a ^
    //    size_t offset = i+1;
    //    next = getSafeCharacter(iCmdLine, offset);
    //    while( next == '^' )
    //    {
    //      offset++;
    //      next = getSafeCharacter(iCmdLine, offset);
    //    }
    //    next = getSafeCharacter(iCmdLine, offset);
    //  }
    //  isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isArgumentSeparator(next);
    //  if (isValidEmptyArgument)
    //  {
    //    //insert an empty argument
    //    oArguments.push_back("");
    //  }
    //}
    //else if ( isArgumentSeparator(c) && !inString && !inCaretString )
    //{
    //  //Rule 9 (old).
    //  //argument separator
    //  if (accumulator != "")
    //  {
    //    oArguments.push_back(accumulator);
    //    accumulator = "";
    //  }
    //}
    //else if (c == '\\' && !inString && !inCaretString)
    //{
    //  //Rule 4 (old).
    //  accumulator.push_back(c);
    //}
    //else
    //{
    //  //Rule 11 (old).
    //  //plain text character
    //  accumulator.push_back(c);
    //}

    //next character
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
