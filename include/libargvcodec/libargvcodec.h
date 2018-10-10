// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBARGVCODEC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBARGVCODEC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//#ifdef LIBARGVCODEC_EXPORTS
//#define LIBARGVCODEC_API __declspec(dllexport)
//#else
//#define LIBARGVCODEC_API __declspec(dllimport)
//#endif

#define LIBARGVCODEC_API 
