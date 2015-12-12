// ArgumentLister.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
  FILE * f = fopen("ArgumentLister.log", "w");
  if (!f)
    return 1;

  for(int i=0; i<argc; i++)
  {
    //printf(   "arg%d=%s\n", i, argv[i]);
    fprintf(f, "argv[%d]=%s\n", i, argv[i]);
  }

  fclose(f);
	return 0;
}
