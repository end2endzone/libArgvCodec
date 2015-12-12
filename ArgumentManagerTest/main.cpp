// ArgMgrTest.c pp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "ArgumentManager.h"
#include "gtest_emulator.h"

ArgumentManager gArgumentMgr;

std::string getExePath()
{
  ArgumentManager tmp;
  tmp.init("");
  const char * arg0 = tmp.getArgument(0);
  return std::string(arg0);
}

//#include <string>
//#include <vector>
//typedef std::vector<std::string> StringList;

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
  //gArgumentMgr.init(argc, argv);
  //if ( gArgumentMgr.contains("LIST_ARGUMENTS") )
  //{
  //  for(int i=0; i<argc; i++)
  //  {
  //    printf("arg%d=%s\n", i, argv[i]);
  //  }
	 // return 0;
  //}

  RUN_ALL_TESTS();

  system("PAUSE");
	return 0;
}

