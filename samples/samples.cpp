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

#include "libargvcodec/ArgumentList.h"

int demo_find_arguments(int argc, char **argv)
{
  //The ArgumentList class encapsulates a list of arguments.
  //Call the `init()` method using the application’s `argc` and `argv` parameters.
  //This initialize the instance with a copy of all the parameters.
  libargvcodec::ArgumentList args;
  args.init(argc, argv); //copy the arguments for manipulation
  
  //Find optional application arguments
  bool isVerbose = args.findOption("verbose"); // searching for `-verbose`, `--verbose` or `/verbose`.
  
  //Find mandatory application arguments:
  //  application input file:
  std::string input_file;
  int input_file_index = -1;
  bool has_input_file = args.findValue("input_file", input_file_index, input_file); // searching for `-file=foobar`, `--file=foobar` or `/file=foobar`.
  if (!has_input_file)
  {
    printf("Missing mandatory argument 'input_file'.\n");
    return 1;
  }

  //Search for arguments with values for patterns likes `/name=foo` or `–-repeat=5` using the `findValue()` method.
  int repeat = 0;
  int repeat_index = -1;
  bool has_repeat = args.findValue("repeat", repeat_index, repeat);
  if (!has_repeat)
  {
    printf("Missing mandatory argument 'repeat'.\n");
    return 1;
  }

  return 0;
}

int demo_unknown_arguments(int argc, char **argv)
{
  //This initialize the instance with a copy of all the parameters.
  libargvcodec::ArgumentList args;
  args.init(argc, argv);

  //Find optional application arguments
  bool isVerbose = args.extractOption("verbose");
  
  //Find mandatory application arguments:
  std::string input_file;
  bool has_input_file = args.extractValue("input_file", input_file);
  //do something if 'has_input_file' is false

  int repeat = 0;
  bool has_repeat = args.extractValue("repeat", repeat);
  //do something if 'has_repeat' is false

  //remaining arguments are unknown to the application
  //return an error if user entered unknown arguments
  if (args.getArgc() > 1)
  {
    for(int i=1; i<args.getArgc(); i++)
    {
      const char * unknownArgument = args.getArgument(i);
      printf("Unknown argument '%s'.\n", unknownArgument);
    }
    return 2;
  }

  return 0;
}

int main(int argc, char **argv)
{
  int return_code = 0;

  return_code = demo_find_arguments(argc, argv);
  if (return_code != 0)
    return return_code;

  return_code = demo_unknown_arguments(argc, argv);
  if (return_code != 0)
    return return_code;

  return 0;
}
