#pragma once
#include <vector>

namespace advancedsort
{
  enum SORT_DATA_TYPE
  {
    SORT_DATA_TYPE_C_STR,
    SORT_DATA_TYPE_STD_STR,
    SORT_DATA_TYPE_BOOL,
    SORT_DATA_TYPE_SINT8,
    SORT_DATA_TYPE_UINT8,
    SORT_DATA_TYPE_SINT16,
    SORT_DATA_TYPE_UINT16,
    SORT_DATA_TYPE_SINT32,
    SORT_DATA_TYPE_UINT32,
    SORT_DATA_TYPE_SINT64,
    SORT_DATA_TYPE_UINT64,
    SORT_DATA_TYPE_FLOAT32,
    SORT_DATA_TYPE_FLOAT64,
  };

  struct SORT_ATTRIBUTE
  {
    bool assending;
    const void * valuePtr;
    SORT_DATA_TYPE valueType;
  };

  typedef std::vector<SORT_ATTRIBUTE> SortAttributeList;

  //returns -1 if iPtr1  < iPtr2 when sorting in assending order. Returns  1 otherwise.
  //returns  0 if iPtr1 == iPtr2.
  //returns  1 if iPtr1  > iPtr2 when sorting in assending order. Returns -1 otherwise.
  template <typename T> 
  inline int compareT(const void * iPtr1, const void * iPtr2, bool iAssending)
  {
    const T & actualValue1 = (*(const T *)iPtr1);
    const T & actualValue2 = (*(const T *)iPtr2);
    bool equal = ( actualValue1 == actualValue2 );
    if (equal)
      return 0;
    int result = ( actualValue1 < actualValue2 ? -1 : 1);
    if (!iAssending)
    {
      //invert result
      if (result == 1)
        result = -1;
      else if (result == -1)
        result = 1;
    }
    return result;
  }

  template <typename T> 
  class SortFunctionT {
  public:
    //must be implemented by each program that sorts a custom type
    virtual SortAttributeList getSortAttributes(const T & iObject) = 0;

    inline bool operator() (const T & iObject1, const T & iObject2)
    {
      SortAttributeList attributes1 = getSortAttributes(iObject1);
      SortAttributeList attributes2 = getSortAttributes(iObject2);

      //level 1:
      if (attributes1.size() == attributes2.size())
      {
        //level 2 and over
        for(size_t i=0; i<attributes1.size(); i++)
        {
          const SORT_ATTRIBUTE & attr1 = attributes1[i];
          const SORT_ATTRIBUTE & attr2 = attributes2[i];

          int compareResult = compare(attr1, attr2);
          if (compareResult == 0)
          {
            //both attribute values are equals
            //this attribute is not a good candidate for comparing
          }
          else
          {
            //compare using this attribute's value
            if (compareResult == -1)
              return true;
            else
              return false;
          }
        }

        //all attribute values are equals
        return false; //true is also a possible value
      }
      else
      {
        //attribute count not equal!
        //undefined results possible
        return attributes1.size() < attributes2.size();
      }
    }

  private:
    inline bool isValid(const SORT_ATTRIBUTE & iValue1, const SORT_ATTRIBUTE & iValue2)
    {
      if (iValue1.valueType != iValue2.valueType)
        return false;

      //basic evaluation
      if (iValue1.valuePtr == NULL && iValue2.valuePtr == NULL)
        return true;
      if (iValue1.valuePtr != NULL && iValue2.valuePtr == NULL)
        return false;
      if (iValue1.valuePtr == NULL && iValue2.valuePtr != NULL)
        return false;
      //both are expected to be non-NULL

      return true;
    }

    inline void invert(int & ioValue)
    {
      if (ioValue == 1)
        ioValue = -1;
      else if (ioValue == -1)
        ioValue = 1;
    }

    //returns -1 if iAttr1  < iAttr2 when sorting in assending order. Returns  1 otherwise.
    //returns  0 if iAttr1 == iAttr2.
    //returns  1 if iAttr1  > iAttr2 when sorting in assending order. Returns -1 otherwise.
    inline int compare(const SORT_ATTRIBUTE & iAttr1, const SORT_ATTRIBUTE & iAttr2)
    {
      //check validity and NULL values
      if (!isValid(iAttr1, iAttr2))
        return 0; //define both as equals / undefined sort order

      switch(iAttr1.valueType)
      {
      case SORT_DATA_TYPE_C_STR:
        {
          const char * tmpValue1 = (const char *)iAttr1.valuePtr;
          const char * tmpValue2 = (const char *)iAttr2.valuePtr;
          return compareT<std::string>(&std::string(tmpValue1), &std::string(tmpValue2), iAttr1.assending);
        }
        break;
      case SORT_DATA_TYPE_STD_STR:
        return compareT<std::string>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_BOOL:
        return compareT<bool>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_SINT8:
        return compareT<char>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_UINT8:
        return compareT<unsigned char>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_SINT16:
        return compareT<short>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_UINT16:
        return compareT<unsigned short>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_SINT32:
        return compareT<int>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_UINT32:
        return compareT<unsigned int>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_SINT64:
        return compareT<__int64>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_UINT64:
        return compareT<unsigned __int64>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_FLOAT32:
        return compareT<float>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      case SORT_DATA_TYPE_FLOAT64:
        return compareT<double>(iAttr1.valuePtr, iAttr2.valuePtr, iAttr1.assending);
        break;
      default:
        return 0; //define both as equals / undefined sort order
      };
    }
  }; 

};
