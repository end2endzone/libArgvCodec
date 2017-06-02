// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBARGVCODECVALIDATOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBARGVCODECVALIDATOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBARGVCODECVALIDATOR_EXPORTS
#define LIBARGVCODECVALIDATOR_API __declspec(dllexport)
#else
#define LIBARGVCODECVALIDATOR_API __declspec(dllimport)
#endif

/// <summary>
/// Macro to export a stl::vector template
/// </summary>
#define VALIDATOR_EXPORT_STL_VECTOR( apimacro, vectortype ) /*\
  template class apimacro std::allocator< vectortype >; \
  template class apimacro std::vector<vectortype, std::allocator< vectortype > >;*/
