#ifndef MACROS_H
#define MACROS_H

#define SAFE_DELETE(var)        {if (var) delete   var; var = NULL;}
#define SAFE_DELETE_ARRAY(var)  {if (var) delete[] var; var = NULL;}

#ifdef _MSC_VER
  #define SAFE_WARNING_DISABLE(value) __pragma( warning(push) ) __pragma( warning(disable: value)) 
  #define SAFE_WARNING_RESTORE() __pragma( warning(pop) )
#else
  #define SAFE_WARNING_DISABLE(value) ;
  #define SAFE_WARNING_RESTORE() ;
#endif

#endif //MACROS_H