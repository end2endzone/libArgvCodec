// ArgMgr.cpp : Defines the exported functions for the DLL application.
//

#include "targetver.h"

#include "ArgumentManager.h"
#include "utils.h"
#include "os.h"
#include <assert.h>

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
    //else if (c == '\"')
    //{
    //  arg.append( (2*numBackslashes) + 1, '\\' );
    //  arg.append( 1, '\"' );
    //  numBackslashes = 0;
    //}
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

  //deal with "
  //strReplace(arg, "\"", "^\"");

  //deal with \ 
  //strReplace(arg, "\\", "^\\");

  ////deal with spaces
  //if (arg.find(" ") != std::string::npos)
  //{
  //  arg.insert(arg.begin(), 1, '\"');
  //  arg.push_back('\"');
  //}

        //// Enquote, doubling last sequence of backslashes ==> "\\share\\\"some folder\"\\"
        //int numTrailingBackslashes = 0;
        //for (int i = arg.size() - 1; i > 0; --i)
        //{
        //    if (arg[i] != '\\')
        //    {
        //        numTrailingBackslashes = arg.size() - 1 - i;
        //        break;
        //    }
        //}
        //arg.append(numTrailingBackslashes, '\\');


  //ici




  //Samples:
  //  argument's value        escape version
  //  \"hello\"               \\\"hello\\\"

  //check "My attempt at escaping" from http://stackoverflow.com/questions/2393384/escape-string-for-process-start



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



void foo1()
{
  //comment separator
}

//std::string ArgumentManager::getCommandLineArgument(int iIndex)
//{
//  //http://blogs.msdn.com/b/twistylittlepassagesallalike/archive/2011/04/23/everyone-quotes-arguments-the-wrong-way.aspx
//  //http://stackoverflow.com/questions/2393384/escape-string-for-process-start
//  //http://stackoverflow.com/questions/5510343/escape-command-line-arguments-in-c-sharp/6040946#6040946
//
//
//  //NOT IMPLEMENTED
//  //TODO: IMPLEMENT ArgumentManager::getCommandLine()
//
//  const char * plainArgument = getArgument(iIndex);
//
//  //escape special shell characters
//  std::string escapedArg = plainArgument;
//  //strReplace(escapedArg, "^", "^^");
//  //strReplace(escapedArg, "&", "^&");
//  //strReplace(escapedArg, "|", "^|");
//  //strReplace(escapedArg, "(", "^(");
//  //strReplace(escapedArg, ")", "^)");
//  //strReplace(escapedArg, "<", "^<");
//  //strReplace(escapedArg, ">", "^>");
//  //strReplace(escapedArg, "%", "^%");
//  //strReplace(escapedArg, "!", "^!");
//
//  //copy escapedArg into arg dealing with \ and " characters.
//  std::string arg;
//  int numBackslashes = 0;
//  //for each characters
//  for(size_t i=0; i<escapedArg.size(); i++)
//  {
//    char c = escapedArg[i];
//
//    if (c == '^')
//    {
//      //already excaped
//      arg.append( 1, c );
//      arg.append( 1, escapedArg[i+1] );
//
//      i++; //skip next char
//    }
//    else if (c == '\"')
//    {
//      arg.append( (2*numBackslashes) + 1, '\\' );
//      arg.append( 1, '\"' );
//      numBackslashes = 0;
//    }
//    else if(c == '\\')
//    {
//      numBackslashes++;
//    }
//    else
//    {
//      arg.append( numBackslashes, '\\' );
//      arg.append( 1, c );
//      numBackslashes = 0;
//    }
//  }
//  arg.append( numBackslashes, '\\' );
//
//  //deal with "
//  //strReplace(arg, "\"", "^\"");
//
//  //deal with \ 
//  //strReplace(arg, "\\", "^\\");
//
//  ////deal with spaces
//  //if (arg.find(" ") != std::string::npos)
//  //{
//  //  arg.insert(arg.begin(), 1, '\"');
//  //  arg.push_back('\"');
//  //}
//
//        //// Enquote, doubling last sequence of backslashes ==> "\\share\\\"some folder\"\\"
//        //int numTrailingBackslashes = 0;
//        //for (int i = arg.size() - 1; i > 0; --i)
//        //{
//        //    if (arg[i] != '\\')
//        //    {
//        //        numTrailingBackslashes = arg.size() - 1 - i;
//        //        break;
//        //    }
//        //}
//        //arg.append(numTrailingBackslashes, '\\');
//
//
//  //ici
//
//  //deal with spaces
//  if (arg.find(" ") != std::string::npos)
//  {
//    arg.insert(arg.begin(), 1, '\"');
//    arg.push_back('\"');
//  }
//
//
//
//  //Samples:
//  //  argument's value        escape version
//  //  \"hello\"               \\\"hello\\\"
//
//  //check "My attempt at escaping" from http://stackoverflow.com/questions/2393384/escape-string-for-process-start
//
//
//  //deal with empty argument
//  if (arg == "")
//  {
//    arg = "\"\"";
//  }
//
//  return arg;
//}

void foo2()
{
  //comment separator
}

//std::string ArgumentManager::getCommandLineArgument(int iIndex)
//{
//  char * plainArgument = getArgument(iIndex);
//
//  std::string arg = plainArgument;
//
//  //escape special shell characters
//  strReplace(arg, "^", "^^");
//  strReplace(arg, "&", "^&");
//  strReplace(arg, "|", "^|");
//  strReplace(arg, "(", "^(");
//  strReplace(arg, ")", "^)");
//  strReplace(arg, "<", "^<");
//  strReplace(arg, ">", "^>");
//  strReplace(arg, "%", "^%");
//  strReplace(arg, "!", "^!");
//
//  strReplace(arg, "\"", "\\^\"");
//
//  //quot the argument
//  //arg.insert(arg.begin(), 1, '\"');
//  //arg.push_back('\"');
//  arg.insert(0, "^\"");
//  arg.append("^\"");
//
//  return arg;
//}

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
  //the last element of argv must be a NULL string
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
  //Samples (tested with cmd.exe):
  //
  //  [command line]                        [arg#1]               [arg#2]                 [arg#3]
  //
  //  aaa"                          =>      aaa
  //  aaa\"                         =>      aaa"
  //  aaa\\"                        =>      aaa\
  //  aaa\\\"                       =>      aaa\"
  //  "aaa\"                        =>      aaa"
  //  "aaa\\"                       =>      aaa\
  //  "aaa\\\"                      =>      aaa\"
  //  a"bc                          =>      abc
  //  a"bc\                         =>      abc\
  //  a"bc\\                        =>      abc\\
  //  a"bc\\\                       =>      abc\\\
  //  a"bc\"d                       =>      abc"d
  //  a"bc\\ ddd" e                 =>      abc\\ ddd             e
  //  a"bc\"\ ddd" e                =>      abc"\ ddd             e
  //  ab\\c d                       =>      ab\\c                 d
  //  ab\\ c                        =>      ab\\                  c
  //  \\ab c                        =>      \\ab                  c
  //  a ""bc\"o" e"                 =>      a                     bc"o e
  //  a "bc\"o" e"                  =>      a                     bc"o                    e
  //  "a"""b                        =>      a"b
  //  "a"""b"                       =>      a"b
  //  "a\"b"                        =>      a"b
  //  "a""" b"                      =>      a"                    b
  //  """                           =>      "
  //  """"                          =>      "
  //  """""                         =>      ""
  //  """"""                        =>      ""
  //  ^"ab cd\^"^&echo foo^"        =>      ab cd"&echo foo
  //   "ab cd\^"^&echo foo^"        =>      ab cd\^&echo          foo
  //  ^"ab cd\^" ee"                =>      ab cd" ee
  //   "ab cd\" ee"                 =>      ab cd" ee
  //   "ab cd\^" ee"                =>      ab cd\^               ee
  //  "a^b" c                       =>      a^b                   c
  //  ^"a^b" c                      =>      ab                    c
  //  ^"ab^" c                      =>      ab                    c
  //  ^"ab^"" c                     =>      ab" c
  //  a \bb c                       =>      a                     \bb                     c
  //  a \"bb c" d                   =>      a                     "bb                     c d
  //  a ^"bb^" c" d                 =>      a                     bb                      c d
  //  a \"b c                       =>      a                     "b                      c
  //  a""b c                        =>      ab                    c
  //  a\"b c                        =>      a"b                   c
  //  "a""b" c                      =>      a"b                   c
  //  "a\"b" c                      =>      a"b                   c
  //  a ^"" b                       =>      a                                             b
  //  a ^"^" b                      =>      a                                             b
  //  ^"" a                         =>                            a
  //  ^"^" a                        =>                            a
  //  ^"test\^"^&whoami^"           =>      test"&whoami
  //  ^"test\\^"^&whoami^"          =>      test\&whoami
  //  ^"test\"^&whoami^"            =>      test"^&whoami^
  //  a\\\\"b                       =>      a\\b
  //  ^"a\\\\"b"                    =>      a\\b
  //  ^"a\^\^\\"b"                  =>      a\\b
  //  ^"a\^\\^\"b"                  =>      a\\b
  //  ^"a\^\\\^"b"                  =>      a\\b
  //
  //Rules:
  // 1. " character that are encountered starts/ends a string and ommited in output. [NOTE: MUST REVIEW NEXT STATEMENT] If they are juxtaposed to other arguments, they form the same argument.
  // 2. " character must be escaped with \" (or escaped with "") and does not ends the string. (if not escaped, they act as string starts/ends commands. See rule #1). Characters escaped with \" can be seen inside or outside strings but characters escaped with "" can be *ONLY* be seen inside a string.
  // 3. \ character [NOT SURE ABOUT THIS: when inside a string] must be escaped with \\ *ONLY* if they precedes a [NOTE: NOT SURE unescaped] " character. (string or caret-string termination). ie: "te\\\"st" ==> te\"st     "te\\\" st" ==> te\" st
  // 4. \ character must be read as plain text when outside a string.
  // 5. ^ character must be read as plain text when inside a string. (not caret-string) This is also true for all other shell characters.
  // 6. ^" sequence starts/ends a caret-string. Caret-strings can also be terminated with an unescaped " character.
  // 7. " character must be escaped with \^" inside caret-string. [NOTE: THE FOLLOWING MAY BE OMMITED] ........... Only strings that starts with ^" (caret-strings) can interpret ^ as a special meta-character. Normal strings will see ^ as a plain character.
  // 8. ^ characters outside a string (or inside a caret-string) will escape a normal or shell character ( like &,<,>,(,),|,% or ! ) and which will read as plain text. The ^ character can also be ommited to make a known sequence. ie: \\\^" for \\\"
  // 9. [space] or tabs characters act as argument separators when outside a string or caret-string
  //10. Empty arguments must be specified with "" and must not be bounded by separators (except when positioned as the first/last argument)
  //11. All other characters must be read as plain text.
  //

  oArguments.clear();

  std::string accumulator;

  const std::string cmdLineStr = iCmdLine;

  bool inString = false;
  bool inCaretString = false;
  bool isValidEmptyArgument = false;
  //bool isStringJuxtaposed = false;

  for(size_t i=0; i<cmdLineStr.size(); i++)
  {
    char c = iCmdLine[i];

    bool isLastCharacter = !(i+1<cmdLineStr.size());
    size_t numBackSlashes = 0;
    size_t backSlashSequenceLength = 0;

    if ( !inString && !inCaretString && matchesSequence(iCmdLine, i, "^\""))
    {
      //Rule 6.
      //new caret-string
      inString = false;
      inCaretString = true;

      //Rule 10. Validate isEmptyArgumentString
      char previous = getSafeCharacter(iCmdLine, i-1);
      isValidEmptyArgument = isArgumentSeparator(previous);

      i=i+1; //skip next character
    }
    else if ( !inString && inCaretString && matchesSequence(iCmdLine, i, "^\""))
    {
      //Rule 6.
      //end caret-string
      inString = false;
      inCaretString = false;

      //Rule 10. Validate isEmptyArgumentString
      char next = getSafeCharacter(iCmdLine, i+1);
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isArgumentSeparator(next);
      if (isValidEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }

      i=i+1; //skip next character
    }
    else if (c == '^' && inString && !inCaretString)
    {
      //Rule 5.
      accumulator.push_back(c);
    }
    else if (c == '^' && (!inString || !inCaretString) )
    {
      //Rule 8.
      //skip this character
    }
    else if (c == '\"' && !inString && !inCaretString)
    {
      //Rule 1.
      //new string
      inString = true;
      inCaretString = false;

      //Rule 10. Validate isEmptyArgumentString
      char previous = getSafeCharacter(iCmdLine, i-1);
      isValidEmptyArgument = isArgumentSeparator(previous);

      //ie:
      //  a"b  c" d
      //  a "b   c"d
    }
    else if ( matchesSequence(iCmdLine, i, "\\^\"") && (inCaretString || !inString) )
    {
      //Rule 2.
      // for \^" character sequence inside a caret-string
      accumulator.push_back('\"');
      i=i+2; //skip next character
    }
    else if ( matchesSequence(iCmdLine, i, "\\\"") )
    {
      //Rule 2.
      // for \" character sequence outside/inside a string or caret-string
      accumulator.push_back('\"');
      i=i+1; //skip next character
    }
    else if ( matchesBackSlashDblQuoteSequence(iCmdLine, i, numBackSlashes, backSlashSequenceLength, inString, inCaretString) )
    {
      //Rule 3.
      // for \\" character sequence (or any combination like \\\" or \\\\" )
      size_t numEscapedBackSlashes = numBackSlashes/2;
      std::string s;
      s.append(numEscapedBackSlashes, '\\');

      accumulator.append(s);



      //i=i+(numEscapedBackSlashes*2)-1; //skip escaped \ characters (but not the last \ if odd backslashes are found) but not the " character
      i=i+backSlashSequenceLength-1; //skip escaped \ characters (but not the last \ if odd backslashes are found) but not the " character
    }
    else if ( (inString || inCaretString) && matchesSequence(iCmdLine, i, "\"\"") )
    {
      //Rule 2.
      // for "" character sequence inside a string or caret-string
      accumulator.push_back('\"');
      i=i+1; //skip next character
    }
    else if (c == '\"' && (inString || inCaretString) )
    {
      //Rule 1.
      //ends a new string or caret-string
      inString = false;
      inCaretString = false;

      //Rule 10. Validate isEmptyArgumentString
      char next = getSafeCharacter(iCmdLine, i+1);
      isValidEmptyArgument = isValidEmptyArgument && accumulator.size() == 0 && isArgumentSeparator(next);
      if (isValidEmptyArgument)
      {
        //insert an empty argument
        oArguments.push_back("");
      }
    }
    else if ( isArgumentSeparator(c) && !inString && !inCaretString )
    {
      //Rule 9.
      //argument separator
      if (accumulator != "")
      {
        oArguments.push_back(accumulator);
        accumulator = "";
      }
    }
    else if (c == '\\' && !inString && !inCaretString)
    {
      //Rule 4.
      accumulator.push_back(c);
    }
    else
    {
      //Rule 11.
      //plain text character
      accumulator.push_back(c);
    }

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


//bool /*THIS IS DISABLED CODE ArgumentManager::parseCmdLine*/aaa(const char * iCmdLine, StringList & oArguments)
//{
////#define PARSECMDLINE_DEBUG
//
//  oArguments.clear();
//
//  std::string accumulator;
//
//  //Remove ^ characters within the string.
//  //Caret characters are used to pass arguments from cmd.exe to foo.exe.
//  //They are not required once foo.exe has parsed the string
//  static const char * caretToken = "{DOUBLECARETTOKEN}";
//  std::string cmdLineTmp = iCmdLine;
//  strReplace(cmdLineTmp, "^^", caretToken);
//  strReplace(cmdLineTmp, "^", "");//remove all other single carret
//  strReplace(cmdLineTmp, caretToken, "^^");
//
//  const std::string cmdLineStr = cmdLineTmp;
//  const char * cmdLine = cmdLineStr.c_str();
//
//  bool inString = false;
//
//  char previous = '\0';
//
//  //Build an array that matches string's length to help mapping quotes
//  enum CHARACTER_CODES
//  {
//    undefined,        //skipped/unprocessed characters
//    separator,        //a space
//    opening,          //opening " character
//    closing,          //closing " character
//    plain,
//    escapeCommand,    //for \ character which escapes the following character
//    escapedQuote,     //for " escaped characters
//    escapedBackslash, //for \ escaped characters
//    literalCommand,   //for ^ character which escapes the following character
//    literalCharacter, //for characters escaped with ^
//    sequence2,        //for handling special sequence of 2 characters long (including current character)
//    sequence3,        //for handling special sequence of 3 characters long (including current character)
//    sequence4,        //for handling special sequence of 4 characters long (including current character)
//  };
//  CHARACTER_CODES * codes = new CHARACTER_CODES[cmdLineStr.size()];
//  for(size_t i=0; i<cmdLineStr.size(); i++)
//  {
//    codes[i] = undefined;
//  }
//
//  bool openingQuotePossibleEmptyArgument = false;
//  bool closingQuotePossibleEmptyArgument = false;
//
//  for(size_t i=0; i<cmdLineStr.size(); i++)
//  {
//    char c = cmdLine[i];
//
//    bool isLastCharacter = !(i+1<cmdLineStr.size());
//
//    if ( matchesSequence(cmdLine, i, "\"\"\"") ) // for """ character sequence
//    {
//      //must be defined as skipped, plain, opening
//      codes[i  ] = sequence3;
//      codes[i+1] = plain;
//
//      accumulator.push_back('\"');
//
//      i=i+2; //skip next 2 characters
//    }
//    else if ( matchesSequence(cmdLine, i, "^^") )
//    {
//      //escaped caret
//      codes[i  ] = literalCommand;
//      codes[i+1] = literalCharacter;
//
//      //keep next ^ character as plain text
//      accumulator.push_back( '^' );
//
//      i++; //skip next character
//    }
//    if (c == '^')
//    {
//      codes[i] = literalCommand;
//      //skipped
//    }
//    else if (c == '\"')
//    {
//      //deal with " characters...
//
//      if (getSafeCharacter(cmdLine, i-1) == '\"' && codes[i-1] == closing) //quote directly follows closer
//      {
//        //plain text character
//        codes[i] = plain;
//        accumulator.push_back(c);
//      }
//      else if ( !inString )
//      {
//        //opening double-quote
//        codes[i] = opening;
//
//        //starting a new string
//        inString = true;
//
//        //check if this opening quote could be an empty parameter ?
//        char previous = getSafeCharacter(cmdLine, i-1);
//        openingQuotePossibleEmptyArgument = (previous == '\0' || previous == ' ' || previous == '\t');
//
//        //forget about the " character
//      }
//      else if ( inString )
//      {
//        //closing double-quote
//        codes[i] = closing;
//
//        //ends a new string
//        inString = false;
//
//        //check if this closing quote could be an empty parameter ?
//        char next = getSafeCharacter(cmdLine, i+1);
//        closingQuotePossibleEmptyArgument = (next == '\0' || next == ' ' || next == '\t');
//
//        bool forceFlush = openingQuotePossibleEmptyArgument && closingQuotePossibleEmptyArgument;
//
//        //flush accumulator if required. Deals with arguments like: a "" b  where "" is an actual empty argument
//        if ( forceFlush )
//        {
//          oArguments.push_back(accumulator);
//          accumulator = "";
//        }
//
//        forceFlush = false;
//
//        //forget about the " character
//      }
//
//    }
//    else if ( matchesSequence(cmdLine, i, "\\\"") )
//    {
//      //escaped double-quote
//      codes[i  ] = escapeCommand;
//      codes[i+1] = escapedQuote;
//
//      //keep next " as plain character
//      accumulator.push_back('\"');
//
//      i++; //skip next character
//    }
//    else if ( matchesSequence(cmdLine, i, "\\\\\\\\") ) //sequence of \\\\ consecutive
//    {
//      //escaped backslash
//      codes[i  ] = sequence4;
//      codes[i+1] = escapedBackslash;
//      codes[i+2] = escapeCommand;
//      codes[i+3] = escapedBackslash;
//
//      //add escaped characters
//      accumulator.push_back('\\');
//      accumulator.push_back('\\');
//
//      i=i+3; //skip next characters
//    }
//    else if ( matchesSequence(cmdLine, i, "\\\\\\\"") ) //sequence of \\\" consecutive
//    {
//      //escaped backslash
//      codes[i  ] = escapeCommand;
//      codes[i+1] = escapedBackslash;
//
//      //keep next " as plain character
//      accumulator.push_back('\\');
//
//      i++; //skip next character
//    }
//    else if ( matchesSequence(cmdLine, i, "\\\\\"") ) //sequence of \\" consecutive
//    {
//      //escaped backslash
//      codes[i  ] = escapeCommand;
//      codes[i+1] = escapedBackslash;
//
//      //keep next " as plain character
//      accumulator.push_back('\\');
//
//      i++; //skip next character
//    }
//    else if ((c == ' ' || c == '\t') && inString == false)
//    {
//      //separator character
//      codes[i] = separator;
//
//      //flush accumulator;
//      if (accumulator != "")
//      {
//        oArguments.push_back(accumulator);
//        accumulator = "";
//      }
//    }
//    else
//    {
//      //plain text character
//      codes[i] = plain;
//      accumulator.push_back(c);
//    }
//
//    //next character
//  }
//
//  //flush accumulator;
//  if (accumulator != "")
//  {
//    oArguments.push_back(accumulator);
//    accumulator = "";
//  }
//
//#ifdef PARSECMDLINE_DEBUG
//  //DEBUG:
//  for(size_t i=0; i<cmdLineStr.size(); i++)
//  {
//    const char * style = "";
//    switch (codes[i])
//    {
//    case undefined:
//      style = "undefined";
//      break;
//    case separator:
//      style = "separator";
//      break;
//    case opening:
//      style = "opening";
//      break;
//    case closing:
//      style = "closing";
//      break;
//    case plain:
//      style = "plain";
//      break;
//    case escapeCommand:
//      style = "escapeCommand";
//      break;
//    case escapedQuote:
//      style = "escapedQuote";
//      break;
//    case escapedBackslash:
//      style = "escapedBackslash";
//      break;
//    case literalCommand:
//      style = "literalCommand";
//      break;
//    case literalCharacter:
//      style = "literalCharacter";
//      break;
//    case sequence2:
//      style = "sequence2";
//      break;
//    case sequence3:
//      style = "sequence3";
//      break;
//    case sequence4:
//      style = "sequence4";
//      break;
//    default :
//      style = "????";
//      break;
//    };
//    printf("DEBUG: %c => %s\n", cmdLine[i], style);
//  }
//#endif
//
//  delete[] codes;
//  codes = NULL;
//
//  return true;
//}
//
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
