// ArgMgr.cpp : Defines the exported functions for the DLL application.
//

#include "targetver.h"

#include "ArgumentManager.h"
#include "utils.h"
#include "os.h"

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

  char * plainArgument = getArgument(iIndex);

  //escape special shell characters
  std::string escapedArg = plainArgument;
  strReplace(escapedArg, "^", "^^");
  strReplace(escapedArg, "&", "^&");
  strReplace(escapedArg, "|", "^|");
  strReplace(escapedArg, "(", "^(");
  strReplace(escapedArg, ")", "^)");
  strReplace(escapedArg, "<", "^<");
  strReplace(escapedArg, ">", "^>");
  strReplace(escapedArg, "%", "^%");
  strReplace(escapedArg, "!", "^!");

  //copy escapedArg into arg dealing with \ and " characters.
  std::string arg;
  int numBackslashes = 0;
  //for each characters
  for(size_t i=0; i<escapedArg.size(); i++)
  {
    char c = escapedArg[i];

    if (c == '^')
    {
      //already excaped
      arg.append( 1, c );
      arg.append( 1, escapedArg[i+1] );

      i++; //skip next char
    }
    else if (c == '\"')
    {
      arg.append( (2*numBackslashes) + 1, '\\' );
      arg.append( 1, '\"' );
      numBackslashes = 0;
    }
    else if(c == '\\')
    {
      numBackslashes++;
    }
    else
    {
      arg.append( numBackslashes, '\\' );
      arg.append( 1, c );
      numBackslashes = 0;
    }
  }
  arg.append( numBackslashes, '\\' );

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

        // Enquote, doubling last sequence of backslashes ==> "\\share\\\"some folder\"\\"
        int numTrailingBackslashes = 0;
        for (int i = arg.size() - 1; i > 0; --i)
        {
            if (arg[i] != '\\')
            {
                numTrailingBackslashes = arg.size() - 1 - i;
                break;
            }
        }
        arg.append(numTrailingBackslashes, '\\');


  //ici

  //deal with spaces
  if (arg.find(" ") != std::string::npos)
  {
    arg.insert(arg.begin(), 1, '\"');
    arg.push_back('\"');
  }



  //Samples:
  //  argument's value        escape version
  //  \"hello\"               \\\"hello\\\"

  //check "My attempt at escaping" from http://stackoverflow.com/questions/2393384/escape-string-for-process-start


  //deal with empty argument
  if (arg == "")
  {
    arg = "\"\"";
  }

  return arg;
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


bool ArgumentManager::parseCmdLine(const char * iCmdLine, StringList & oArguments)
{
//#define PARSECMDLINE_DEBUG

  oArguments.clear();

  std::string accumulator;

  //Remove ^ characters within the string.
  //Caret characters are used to pass arguments from cmd.exe to foo.exe.
  //They are not required once foo.exe has parsed the string
  static const char * caretToken = "{DOUBLECARETTOKEN}";
  std::string cmdLineTmp = iCmdLine;
  strReplace(cmdLineTmp, "^^", caretToken);
  strReplace(cmdLineTmp, "^", "");//remove all other single carret
  strReplace(cmdLineTmp, caretToken, "^^");

  const std::string cmdLineStr = cmdLineTmp;
  const char * cmdLine = cmdLineStr.c_str();

  bool inString = false;

  char previous = '\0';

  //Build an array that matches string's length to help mapping quotes
  enum CHARACTER_CODES
  {
    undefined,        //skipped/unprocessed characters
    separator,        //a space
    opening,          //opening " character
    closing,          //closing " character
    plain,
    escapeCommand,    //for \ character which escapes the following character
    escapedQuote,     //for " escaped characters
    escapedBackslash, //for \ escaped characters
    literalCommand,   //for ^ character which escapes the following character
    literalCharacter, //for characters escaped with ^
    sequence2,        //for handling special sequence of 2 characters long (including current character)
    sequence3,        //for handling special sequence of 3 characters long (including current character)
    sequence4,        //for handling special sequence of 4 characters long (including current character)
  };
  CHARACTER_CODES * codes = new CHARACTER_CODES[cmdLineStr.size()];
  for(size_t i=0; i<cmdLineStr.size(); i++)
  {
    codes[i] = undefined;
  }

  bool openingQuotePossibleEmptyArgument = false;
  bool closingQuotePossibleEmptyArgument = false;

  for(size_t i=0; i<cmdLineStr.size(); i++)
  {
    char c = cmdLine[i];

    bool isLastCharacter = !(i+1<cmdLineStr.size());

    if ( matchesSequence(cmdLine, i, "\"\"\"") ) // for """ character sequence
    {
      //must be defined as skipped, plain, opening
      codes[i  ] = sequence3;
      codes[i+1] = plain;

      accumulator.push_back('\"');

      i=i+2; //skip next 2 characters
    }
    else if ( matchesSequence(cmdLine, i, "^^") )
    {
      //escaped caret
      codes[i  ] = literalCommand;
      codes[i+1] = literalCharacter;

      //keep next ^ character as plain text
      accumulator.push_back( '^' );

      i++; //skip next character
    }
    if (c == '^')
    {
      codes[i] = literalCommand;
      //skipped
    }
    else if (c == '\"')
    {
      //deal with " characters...

      if (getSafeCharacter(cmdLine, i-1) == '\"' && codes[i-1] == closing) //quote directly follows closer
      {
        //plain text character
        codes[i] = plain;
        accumulator.push_back(c);
      }
      else if ( !inString )
      {
        //opening double-quote
        codes[i] = opening;

        //starting a new string
        inString = true;

        //check if this opening quote could be an empty parameter ?
        char previous = getSafeCharacter(cmdLine, i-1);
        openingQuotePossibleEmptyArgument = (previous == '\0' || previous == ' ' || previous == '\t');

        //forget about the " character
      }
      else if ( inString )
      {
        //closing double-quote
        codes[i] = closing;

        //ends a new string
        inString = false;

        //check if this closing quote could be an empty parameter ?
        char next = getSafeCharacter(cmdLine, i+1);
        closingQuotePossibleEmptyArgument = (next == '\0' || next == ' ' || next == '\t');

        bool forceFlush = openingQuotePossibleEmptyArgument && closingQuotePossibleEmptyArgument;

        //flush accumulator if required. Deals with arguments like: a "" b  where "" is an actual empty argument
        if ( forceFlush )
        {
          oArguments.push_back(accumulator);
          accumulator = "";
        }

        forceFlush = false;

        //forget about the " character
      }

    }
    else if ( matchesSequence(cmdLine, i, "\\\"") )
    {
      //escaped double-quote
      codes[i  ] = escapeCommand;
      codes[i+1] = escapedQuote;

      //keep next " as plain character
      accumulator.push_back('\"');

      i++; //skip next character
    }
    else if ( matchesSequence(cmdLine, i, "\\\\\\\\") ) //sequence of \\\\ consecutive
    {
      //escaped backslash
      codes[i  ] = sequence4;
      codes[i+1] = escapedBackslash;
      codes[i+2] = escapeCommand;
      codes[i+3] = escapedBackslash;

      //add escaped characters
      accumulator.push_back('\\');
      accumulator.push_back('\\');

      i=i+3; //skip next characters
    }
    else if ( matchesSequence(cmdLine, i, "\\\\\\\"") ) //sequence of \\\" consecutive
    {
      //escaped backslash
      codes[i  ] = escapeCommand;
      codes[i+1] = escapedBackslash;

      //keep next " as plain character
      accumulator.push_back('\\');

      i++; //skip next character
    }
    else if ( matchesSequence(cmdLine, i, "\\\\\"") ) //sequence of \\" consecutive
    {
      //escaped backslash
      codes[i  ] = escapeCommand;
      codes[i+1] = escapedBackslash;

      //keep next " as plain character
      accumulator.push_back('\\');

      i++; //skip next character
    }
    else if ((c == ' ' || c == '\t') && inString == false)
    {
      //separator character
      codes[i] = separator;

      //flush accumulator;
      if (accumulator != "")
      {
        oArguments.push_back(accumulator);
        accumulator = "";
      }
    }
    else
    {
      //plain text character
      codes[i] = plain;
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

#ifdef PARSECMDLINE_DEBUG
  //DEBUG:
  for(size_t i=0; i<cmdLineStr.size(); i++)
  {
    const char * style = "";
    switch (codes[i])
    {
    case undefined:
      style = "undefined";
      break;
    case separator:
      style = "separator";
      break;
    case opening:
      style = "opening";
      break;
    case closing:
      style = "closing";
      break;
    case plain:
      style = "plain";
      break;
    case escapeCommand:
      style = "escapeCommand";
      break;
    case escapedQuote:
      style = "escapedQuote";
      break;
    case escapedBackslash:
      style = "escapedBackslash";
      break;
    case literalCommand:
      style = "literalCommand";
      break;
    case literalCharacter:
      style = "literalCharacter";
      break;
    case sequence2:
      style = "sequence2";
      break;
    case sequence3:
      style = "sequence3";
      break;
    case sequence4:
      style = "sequence4";
      break;
    default :
      style = "????";
      break;
    };
    printf("DEBUG: %c => %s\n", cmdLine[i], style);
  }
#endif

  delete[] codes;
  codes = NULL;

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
