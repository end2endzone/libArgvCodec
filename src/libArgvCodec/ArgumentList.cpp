/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#include "libargvcodec/ArgumentList.h"
#include "rapidassist/strings.h"
#include <assert.h>
#include <vector>
#include <algorithm>

static const bool gDefaultCaseSensitive = true;

namespace libargvcodec
{

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
  //safe delete the array
  if (mArgv)
    delete[] mArgv;
  mArgv = NULL;
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

const char * ArgumentList::getArgument(int iIndex) const
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
  //safe delete the array
  if (mArgv)
    delete[] mArgv;
  mArgv = NULL;

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

int ArgumentList::findIndex(const char * iValue, bool iCaseSensitive) const
{
  if (iValue == NULL)
    return -1;
  std::string uppercaseValue = ra::strings::uppercase(iValue);
  for(size_t i=0; i<mArguments.size(); i++)
  {
    const std::string & arg = mArguments[i];
    if (    ( iCaseSensitive && arg == iValue) ||
            (!iCaseSensitive && ra::strings::uppercase(arg) == uppercaseValue)    )
      return (int)i;
  }
  return -1;
}

int ArgumentList::findIndex(const char * iValue) const
{
  return findIndex(iValue, gDefaultCaseSensitive);
}

bool ArgumentList::contains(const char * iValue, bool iCaseSensitive) const
{
  if (iValue == NULL)
    return false;
  int pos = findIndex(iValue, iCaseSensitive);
  return pos != -1;
}

bool ArgumentList::contains(const char * iValue) const
{
  return contains(iValue, gDefaultCaseSensitive);
}

bool ArgumentList::findOption2(const char * iValue, bool iCaseSensitive, int & oIndex) const
{
  oIndex = -1;
  if (iValue == NULL)
    return false;
  oIndex = findIndex(iValue, iCaseSensitive);
  return oIndex != -1;
}

bool ArgumentList::findOption(const char * iValue, int & oIndex) const
{
  return findOption(iValue, gDefaultCaseSensitive, oIndex);
}

bool ArgumentList::findOption(const char * iValue) const
{
  int tmp = 0;
  return findOption(iValue, gDefaultCaseSensitive, tmp);
}

bool ArgumentList::findValue2(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const
{
  oIndex = -1;
  oValue= "";
  if (iValueName == NULL)
    return false;
  std::string valueName = equalize(iValueName);
  std::string uppercaseValueName = ra::strings::uppercase(valueName);
  for(size_t i=0; i<mArguments.size(); i++)
  {
    const std::string & arg = mArguments[i];
    std::string argPrefix = arg.substr(0, valueName.size());
    if (    ( iCaseSensitive && argPrefix == valueName) ||
            (!iCaseSensitive && ra::strings::uppercase(argPrefix) == uppercaseValueName)    )
    {
      oIndex = (int)i;
      oValue = arg.substr(argPrefix.size(), 999999);
      return true;
    }
  }
  return false;
}

bool ArgumentList::findValue(const char * iValueName, bool iCaseSensitive, int & oIndex, int & oValue) const
{
  oIndex = -1;
  oValue = 0;

  int index = 0;
  std::string sValue;
  bool found = findValue(iValueName, iCaseSensitive, index, sValue);
  if (!found)
    return false;
  if (sValue != "")
  {
    oIndex = index;
    oValue = atoi(sValue.c_str());
  }
  return true;
}

bool ArgumentList::findValue(const char * iValueName, int & oIndex, std::string & oValue) const
{
  return findValue(iValueName, gDefaultCaseSensitive, oIndex, oValue);
}

bool ArgumentList::findValue(const char * iValueName, int & oIndex, int & oValue) const
{
  return findValue(iValueName, gDefaultCaseSensitive, oIndex, oValue);
}

bool ArgumentList::findNextValue2(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const
{
  oIndex = -1;
  oValue= "";
  if (iValueName == NULL)
    return false;
  std::string uppercaseValueName = ra::strings::uppercase(iValueName);
  for(size_t i=0; i<mArguments.size(); i++)
  {
    const std::string & arg = mArguments[i];
    if (    ( iCaseSensitive && arg == iValueName) ||
            (!iCaseSensitive && ra::strings::uppercase(arg) == uppercaseValueName)    )
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

bool ArgumentList::findNextValue(const char * iValueName, bool iCaseSensitive, int & oIndex, int & oValue) const
{
  oIndex = -1;
  oValue = 0;

  int index = 0;
  std::string sValue;
  bool found = findNextValue(iValueName, iCaseSensitive, index, sValue);
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
  return findNextValue(iValueName, gDefaultCaseSensitive, oIndex, oValue);
}

bool ArgumentList::findNextValue(const char * iValueName, int & oIndex, int & oValue) const
{
  return findNextValue(iValueName, gDefaultCaseSensitive, oIndex, oValue);
}

bool ArgumentList::extractOption(const char * iValue, bool iCaseSensitive)
{
  if (iValue == NULL)
    return false;
  int index = 0;
  bool found = findOption(iValue, iCaseSensitive, index);
  if (found)
  {
    return this->remove(index);
  }
  return false;
}

bool ArgumentList::extractOption(const char * iValue)
{
  return extractOption(iValue, gDefaultCaseSensitive);
}

bool ArgumentList::extractValue(const char * iValueName, bool iCaseSensitive, std::string & oValue)
{
  oValue = "";
  if (iValueName == NULL)
    return false;
  int index = 0;
  bool found = findValue(iValueName, iCaseSensitive, index, oValue);
  if (found)
  {
    return this->remove(index);
  }
  return false;
}

bool ArgumentList::extractValue(const char * iValueName, bool iCaseSensitive, int & oValue)
{
  oValue = 0;

  std::string sValue;
  bool found = extractValue(iValueName, iCaseSensitive, sValue);
  if (!found)
    return false;
  if (sValue != "")
  {
    oValue = atoi(sValue.c_str());
  }
  return true;
}

std::string ArgumentList::extractValue(const char * iValueName, bool iCaseSensitive)
{
  if (iValueName == NULL)
    return "";
  std::string value;
  bool found = extractValue(iValueName, iCaseSensitive, value);
  if (found)
  {
    return value;
  }
  return "";
}

bool ArgumentList::extractValue(const char * iValueName, std::string & oValue)
{
  return extractValue(iValueName, gDefaultCaseSensitive, oValue);
}

bool ArgumentList::extractValue(const char * iValueName, int & oValue)
{
  return extractValue(iValueName, gDefaultCaseSensitive, oValue);
}

std::string ArgumentList::extractValue(const char * iValueName)
{
  return extractValue(iValueName, gDefaultCaseSensitive);
}

bool ArgumentList::extractNextValue(const char * iValueName, bool iCaseSensitive, std::string & oValue)
{
  oValue = "";
  if (iValueName == NULL)
    return false;
  int index = 0;
  bool found = findNextValue(iValueName, iCaseSensitive, index, oValue);
  if (found)
  {
    bool success = true;
    success = success && this->remove(index); //the option
    success = success && this->remove(index); //the value
    return success;
  }
  return false;
}

bool ArgumentList::extractNextValue(const char * iValueName, bool iCaseSensitive, int & oValue)
{
  oValue = 0;

  std::string sValue;
  bool found = extractNextValue(iValueName, iCaseSensitive, sValue);
  if (!found)
    return false;
  if (sValue != "")
  {
    oValue = atoi(sValue.c_str());
  }
  return true;
}

std::string ArgumentList::extractNextValue(const char * iValueName, bool iCaseSensitive)
{
  if (iValueName == NULL)
    return "";
  std::string value;
  bool found = extractNextValue(iValueName, iCaseSensitive, value);
  if (found)
  {
    return value;
  }
  return "";
}

bool ArgumentList::extractNextValue(const char * iValueName, std::string & oValue)
{
  return extractNextValue(iValueName, gDefaultCaseSensitive, oValue);
}

bool ArgumentList::extractNextValue(const char * iValueName, int & oValue)
{
  return extractNextValue(iValueName, gDefaultCaseSensitive, oValue);
}

std::string ArgumentList::extractNextValue(const char * iValueName)
{
  return extractNextValue(iValueName, gDefaultCaseSensitive);
}

std::string ArgumentList::equalize(const std::string & iValue)
{
  std::string tmp = iValue;
  equalize(tmp);
  return tmp;
}

std::string ArgumentList::equalize(const char * iValue)
{
  std::string tmp = iValue;
  equalize(tmp);
  return tmp;
}

void ArgumentList::equalize(std::string & iValue)
{
  if (!iValue.empty() && iValue[iValue.size()-1] != '=')
  {
    iValue.append(1, '=');
  }
}

const ArgumentList::StringList & ArgumentList::getOptionPrefixes()
{
  return mPrefixes;
}

bool ArgumentList::addOptionPrefix(const char * iValue)
{
  if (iValue != NULL)
  {
    mPrefixes.push_back(std::string(iValue));
    return true;
  }
  return false;
}

bool ArgumentList::removeOptionPrefix(const char * iValue)
{
  StringList::iterator pos = std::find(mPrefixes.begin(), mPrefixes.end(), std::string(iValue));
  if (pos != mPrefixes.end())
  {
    mPrefixes.erase(pos);
    return true;
  }
  return false;
}

void ArgumentList::clearOptionPrefixes()
{
  mPrefixes.clear();
}

//
// Description:
//    Concatenate iNewPrefix with iValueName removing any know prefix from iValueName before concatenation.
//    ie: if newValue is -name=bar and iNewPrefix=='/' then
//    newValue should become /name=bar
//
std::string ArgumentList::rebuildArgumentPrefix(const char * iValueName, const std::string & iNewPrefix) const
{
  std::string newValue = iValueName;

  //remove any known prefix
  for(size_t i=0; i<mPrefixes.size(); i++)
  {
    const std::string & oldPrefix = mPrefixes[i];
    
    //skip empty prefix
    if (oldPrefix.empty())
      continue; 

    //if newValue is -name=bar and iNewPrefix=='/'
    if (newValue.substr(0, oldPrefix.size()) == oldPrefix)
    {
      //remove old prefix from newValue
      newValue.erase(newValue.begin(), newValue.begin()+oldPrefix.size());
    }
  }

  //insert new prefix
  newValue.insert(0, iNewPrefix.c_str());

  return newValue;
}

bool ArgumentList::findOption(const char * iValue, bool iCaseSensitive, int & oIndex) const
{
  bool found = findOption2(iValue, iCaseSensitive, oIndex);
  if (found)
    return true;

  //not found.
  //search with the list of known prefixes...
  for(size_t i=0; i<mPrefixes.size(); i++)
  {
    const std::string & prefix = mPrefixes[i];
      
    std::string newValue = rebuildArgumentPrefix(iValue, prefix);

    //search again
    found = findOption2(newValue.c_str(), iCaseSensitive, oIndex);
    if (found)
      return true;

    //next prefix
  }

  return false; //failed
}

bool ArgumentList::findValue(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const
{
  bool found = findValue2(iValueName, iCaseSensitive, oIndex, oValue);
  if (found)
    return true;

  //not found.
  //search with the list of known prefixes...
  for(size_t i=0; i<mPrefixes.size(); i++)
  {
    const std::string & prefix = mPrefixes[i];
      
    std::string newValue = rebuildArgumentPrefix(iValueName, prefix);

    //search again
    found = findValue2(newValue.c_str(), iCaseSensitive, oIndex, oValue);
    if (found)
      return true;

    //next prefix
  }

  return false; //failed
}

bool ArgumentList::findNextValue(const char * iValueName, bool iCaseSensitive, int & oIndex, std::string & oValue) const
{
  //une fois que c'est confirmer que ca fonctionne bien, deleter ArgumentList.cpp.old

  bool found = findNextValue2(iValueName, iCaseSensitive, oIndex, oValue);
  if (found)
    return true;

  //not found.
  //search with the list of known prefixes...
  for(size_t i=0; i<mPrefixes.size(); i++)
  {
    const std::string & prefix = mPrefixes[i];
      
    std::string newValue = rebuildArgumentPrefix(iValueName, prefix);

    //search again
    found = findNextValue2(newValue.c_str(), iCaseSensitive, oIndex, oValue);
    if (found)
      return true;

    //next prefix
  }

  return false; //failed

}

}; //namespace libargvcodec
