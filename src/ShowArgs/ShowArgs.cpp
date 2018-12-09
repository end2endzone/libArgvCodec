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

#include <stdio.h>
#include <iostream>

#include "rapidassist/console.h"
#include "rapidassist/environment.h"

using namespace std;

//look if the environment variable 'showargs_output' is defined
std::string getFileOutputPath()
{
  std::string tmp;

  tmp = ra::environment::getEnvironmentVariable("showargs_output");
  if (!tmp.empty())
    return tmp;

  tmp = ra::environment::getEnvironmentVariable("SHOWARGS_OUTPUT");
  if (!tmp.empty())
    return tmp;

  tmp = ra::environment::getEnvironmentVariable("showargs_OUTPUT");
  if (!tmp.empty())
    return tmp;

  tmp = ra::environment::getEnvironmentVariable("SHOWARGS_output");
  if (!tmp.empty())
    return tmp;

  return tmp;
}

int main(int argc, char* argv[])
{
  //look for a file output path
  std::string output_file_path = getFileOutputPath();
  bool file_output_enabled = !output_file_path.empty();
  if (file_output_enabled)
  {
    //output to a file
    FILE * f = fopen(output_file_path.c_str(), "w");
    if (f == NULL)
      return 1;

    for(int i=0; i<argc; i++)
    {
      const char * value = argv[i];
      fprintf(f, "arg%d: %s\n", i, value);
    }

    fclose(f);
  }
  else
  {
    //output to stdout
    for(int i=0; i<argc; i++)
    {
      const char * value = argv[i];
      cout << "arg" << i << ": " << value << endl;
    }

    //is this application that spawned a console?
    //if so, then we ask the user to press a key before it goes away
    if (ra::console::hasConsoleOwnership())
    {
      cout << "Press a key to terminate application..." << endl;
      getchar();
    }
  }

	return 0;
}
