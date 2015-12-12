#include "ValidationRuleSorter.h"

ValidationRuleSorter::~ValidationRuleSorter()
{
  //free bools
  for(size_t i=0; i<mBoolInstances.size(); i++)
  {
    bool * b = mBoolInstances[i];
    delete b;
  }
  mBoolInstances.clear();
}

bool * ValidationRuleSorter::newBool(bool iValue)
{
  bool * b = new bool();
  *b = iValue;
  mBoolInstances.push_back(b);
  return b;
}

advancedsort::SortAttributeList ValidationRuleSorter::getSortAttributes(const VALIDATION_RULE_PTR & iObject)
{
  advancedsort::SortAttributeList f;
  advancedsort::SORT_ATTRIBUTE v;

  if (iObject == NULL)
    return f;

  //level 1: isMandatory
  v.assending = false;
  v.valuePtr = newBool(iObject->flags.isMandatory);
  v.valueType = advancedsort::SORT_DATA_TYPE_BOOL;
  f.push_back(v);

  //level 2: hasNextValue
  v.assending = false;
  v.valuePtr = newBool(iObject->flags.hasNextValue);
  v.valueType = advancedsort::SORT_DATA_TYPE_BOOL;
  f.push_back(v);
      
  //level 3: hasValue
  v.assending = false;
  v.valuePtr = newBool(iObject->flags.hasValue);
  v.valueType = advancedsort::SORT_DATA_TYPE_BOOL;
  f.push_back(v);
      
  //level 4: isFlag
  v.assending = false;
  v.valuePtr = newBool(iObject->flags.isFlag);
  v.valueType = advancedsort::SORT_DATA_TYPE_BOOL;
  f.push_back(v);
      
  //level 5: caseSensitive
  v.assending = false;
  v.valuePtr = newBool(iObject->flags.caseSensitive);
  v.valueType = advancedsort::SORT_DATA_TYPE_BOOL;
  f.push_back(v);
      
  //level 6: name
  v.assending = true;
  v.valuePtr = &iObject->argumentName;
  v.valueType = advancedsort::SORT_DATA_TYPE_STD_STR;
  f.push_back(v);
      
  return f;
}
