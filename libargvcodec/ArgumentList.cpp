#include "targetver.h"

#include "ArgumentList.h"
#include "utils.h"
#include "os.h"
#include <assert.h>
#include <vector>

ArgumentList::ArgumentList() :
  mArgv(NULL)
{
}

ArgumentList::ArgumentList(const ArgumentList & iArgumentManager) :
  mArgv(NULL)
{
  (*this) = iArgumentManager;
}

ArgumentList::~ArgumentList()
{
  SAFE_DELETE_ARRAY(mArgv);
}

void ArgumentList::init(int argc, char** argv)
{
  StringList tmp;
  for(int i=0; i<argc; i++)
  {
    tmp.push_back(argv[i]);
  }
  init(tmp);
}

void ArgumentList::init(const StringList & iArguments)
{
  mArguments = iArguments;
  rebuildArgv();
}

char * ArgumentList::getArgument(int iIndex) const
{
  if (!isValid(iIndex))
    return ""; //out of bounds
  return mArgv[iIndex];
}

bool ArgumentList::insert(int iIndex, const char * iValue)
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

bool ArgumentList::insert(const char * iValue)
{
  if (iValue == NULL)
    return false;
  mArguments.push_back(iValue);
  rebuildArgv();
  return true;
}

bool ArgumentList::remove(int iIndex)
{
  if (!isValid(iIndex))
    return false; //out of bounds
  mArguments.erase(mArguments.begin() + iIndex);
  rebuildArgv();
  return true;
}

bool ArgumentList::replace(int iIndex, const char * iValue)
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

int ArgumentList::getArgc() const
{
  return (int)mArguments.size();
}

char** ArgumentList::getArgv() const
{
  return mArgv;
}

const ArgumentList & ArgumentList::operator = (const ArgumentList & iArgumentManager)
{
  this->init(iArgumentManager.mArguments);
  return (*this);
}

bool ArgumentList::operator == (const ArgumentList & iArgumentManager) const
{
  bool equals = (this->mArguments == iArgumentManager.mArguments);
  return equals;
}

bool ArgumentList::operator != (const ArgumentList & iArgumentManager) const
{
  bool notEquals = (this->mArguments != iArgumentManager.mArguments);
  return notEquals;
}

void ArgumentList::rebuildArgv()
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

bool ArgumentList::isValid(int iIndex) const
{
  if (iIndex >= 0 && iIndex < getArgc())
  {
    return true;
  }
  return false;
}

int ArgumentList::findIndex(const char * iValue) const
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

bool ArgumentList::contains(const char * iValue) const
{
  if (iValue == NULL)
    return false;
  int pos = findIndex(iValue);
  return pos != -1;
}

bool ArgumentList::findOption(const char * iValue) const
{
  if (iValue == NULL)
    return false;
  int tmp = 0;
  return findOption(iValue, tmp);
}

bool ArgumentList::findOption(const char * iValue, int & oIndex) const
{
  oIndex = -1;
  if (iValue == NULL)
    return false;
  oIndex = findIndex(iValue);
  return oIndex != -1;
}

bool ArgumentList::findValue(const char * iValueName, int & oIndex, std::string & oValue) const
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

bool ArgumentList::findValue(const char * iValueName, int & oIndex, int & oValue) const
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
    oIndex = index;
    oValue = atoi(sValue.c_str());
  }
  return true;
}

bool ArgumentList::findNextValue(const char * iValueName, int & oIndex, std::string & oValue) const
{
  oIndex = -1;
  oValue= "";
  if (iValueName == NULL)
    return false;
  for(size_t i=0; i<mArguments.size(); i++)
  {
    const std::string & arg = mArguments[i];
    if (arg == iValueName)
    {
      //find option.
      //look for the next argument as the value
      if (i+1 < mArguments.size())
      {
        //good a value is available
        oIndex = (int)i;
        oValue = mArguments[i+1];
        return true;
      }
    }
  }
  return false;
}

bool ArgumentList::findNextValue(const char * iValueName, int & oIndex, int & oValue) const
{
  oIndex = -1;
  oValue = 0;

  int index = 0;
  std::string sValue;
  bool found = findNextValue(iValueName, index, sValue);
  if (!found)
    return false;
  if (sValue != "")
  {
    oIndex = index;
    oValue = atoi(sValue.c_str());
  }
  return true;
}

bool ArgumentList::extractOption(const char * iValue)
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

bool ArgumentList::extractValue(const char * iValueName, std::string & oValue)
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

bool ArgumentList::extractValue(const char * iValueName, int & oValue)
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

std::string ArgumentList::extractValue(const char * iValueName)
{
  if (iValueName == NULL)
    return "";
  std::string value;
  bool found = extractValue(iValueName, value);
  if (found)
  {
    return value;
  }
  return "";
}

bool ArgumentList::extractNextValue(const char * iValueName, std::string & oValue)
{
  oValue = "";
  if (iValueName == NULL)
    return false;
  int index = 0;
  bool found = findNextValue(iValueName, index, oValue);
  if (found)
  {
    bool success = true;
    success = success && this->remove(index); //the option
    success = success && this->remove(index); //the value
    return success;
  }
  return false;
}

bool ArgumentList::extractNextValue(const char * iValueName, int & oValue)
{
  oValue = 0;

  std::string sValue;
  bool found = extractNextValue(iValueName, sValue);
  if (!found)
    return false;
  if (sValue != "")
  {
    oValue = atoi(sValue.c_str());
  }
  return true;
}

std::string ArgumentList::extractNextValue(const char * iValueName)
{
  if (iValueName == NULL)
    return "";
  std::string value;
  bool found = extractNextValue(iValueName, value);
  if (found)
  {
    return value;
  }
  return "";
}
