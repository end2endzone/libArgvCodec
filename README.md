![libArgvCodec logo](docs/libArgvCodec-splashscreen.jpg?raw=true)


# libArgvCodec #
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/libargvcodec.svg)](https://github.com/end2endzone/libargvcodec/releases)

libArgvCodec is a c++ library which properly encode and decode command line arguments. It allows one to properly escape string values to use as command line parameters.


## Status ##

Build:

| Service | Build | Tests |
|----|-------|-------|
| AppVeyor | [![Build status](https://img.shields.io/appveyor/ci/end2endzone/libArgvCodec/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/libargvcodec) | [![Tests status](https://img.shields.io/appveyor/tests/end2endzone/libargvcodec/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/libargvcodec/branch/master/tests) |
| Travis CI | [![Build Status](https://img.shields.io/travis/end2endzone/libArgvCodec/master.svg?logo=travis&style=flat)](https://travis-ci.org/end2endzone/libArgvCodec) |  |

Statistics:

| AppVeyor | Travic CI |
|----------|-----------|
| [![Statistics](https://buildstats.info/appveyor/chart/end2endzone/libargvcodec)](https://ci.appveyor.com/project/end2endzone/libargvcodec/branch/master) | [![Statistics](https://buildstats.info/travisci/chart/end2endzone/libArgvCodec)](https://travis-ci.org/end2endzone/libArgvCodec) |




# Philosophy #

This library tries to solve a decade old problem: how to properly encode command line arguments. Properly encoding string arguments into a long command line should not be a complicated issue. However, many have seen multiple situations where people wrongfully assume that wrapping each argument with double quotes is "safe enough" for most situations. That is simply not true.

The purpose of the library is not to handle *Argument Parsing* and *Argument Validation* : there are already multiple libraries that can do that and most of them does it pretty well.

However, few of them are actually developed to help a user passing arguments from one application to another. The libArgvCodec library provides command line arguments encoder and decoder (codecs) that are designed for this particular purpose.

Transferring local variable values to another application through command line arguments is not something that occurs pretty often but if you ever have to deal with this situation, this library can help you avoid typical encoding pitfalls.




# Features #

The main features of the library are:

* Provides classes for command line argument manipulation.
* Supports basic case sensitive and case insensitive searches within lists of arguments.
* Quickly identify unknown arguments.
* Supports any type of argument prefixes like: slashes, single dash, double dash, etc.
* Encodes multiple arguments into a full command line.
* Decodes a full command line into individual arguments.
* Supports both Windows shell (command prompt) and Windows [CreateProcess()](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682425(v=vs.85).aspx) encoding/decoding.
* Bulletproof application against command line injection attacks.




# Rules #

The list of rules implemented into the library for properly *encoding arguments into a command line* or *splitting a command line into individuals arguments* is documented in the following files:

* [Rules for Windows](Rules.Windows.md)
* [Rules for Linux](Rules.Linux.md)

Please refer to these files for implementation details.




# Usage #

The following instructions show how to use the library with multiple source code examples:



## Encoding & Decoding interfaces ##

The encoding and decoding of command line arguments is with `IArgumentEncoder` and `IArgumentDecoder` interfaces. To interact with the interfaces, the `ArgumentList` class is used as input data or output data.

The library provides the following classes which implements both interfaces:

* `CmdPromptArgumentCodec` handles encoding/decoding for the Windows Command Prompt (shell).
* `CreateProcessArgumentCodec` handles encoding/decoding for the Windows [CreateProcess()](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682425(v=vs.85).aspx) api.
* `TerminalArgumentCodec` handles encoding/decoding for the Linux Terminal (shell).

Using these classes, one can fill up an `ArgumentList` instance and call the `encodeCommandLine()` method to encode all the arguments in the list into a single command line.

Using the same codec classes, one can "split" a command line into multiple arguments using the `decodeCommandLine()` method which returns a valid `ArgumentList` with the result.



## Encoding values into a command line ##

The following code example ask for multiple string values (arguments) and uses `CmdPromptArgumentCodec` and `CreateProcessArgumentCodec` classes to encode the values into the resulting command line:

```cpp
#include <stdio.h>
#include <iostream>

#include "libargvcodec/ArgumentList.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "libargvcodec/CreateProcessArgumentCodec.h"

using namespace libargvcodec;
using namespace std;

int main(int argc, char* argv[])
{
  int numArguments = 0;
  cout << "Enter the number of arguments to encode: ";
  cin >> numArguments;
  cout << endl;

  //capture the remaining ENTER key
  std::string tmp;
  std::getline(std::cin, tmp);

  //validate user input
  if (numArguments < 0)
  {
    cout << "invalid number of arguments" << endl;
    return 1;
  }
  else if (numArguments == 0)
  {
    //nothing to do
    return 0;
  }

  //build the list of arguments
  ArgumentList arglist;
  arglist.insert("foo.exe"); //argv[0] is always the path of the current process

  for(int i=0; i<numArguments; i++)
  {
    cout << "Enter the value of argument #" << i+1 << ": ";

    std::string value;
    std::getline(std::cin, value);

    arglist.insert(value.c_str());
  }
  cout << "done." << endl;
  cout << endl;

  //Encode
  {
    CmdPromptArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "The encoded command line for the Command Prompt is the following:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }
  {
    CreateProcessArgumentCodec codec;
    std::string cmdLine = codec.encodeCommandLine(arglist);
    cout << "The encoded command line for CreateProcess() api is the following:" << endl;
    cout << cmdLine << endl;
    cout << endl;
  }

	return 0;
}
```



## Manipulating an argument list ##

The library supports arguments encapsulation into the `ArgumentList` class. The class is used as a container for arguments. It supports all CRUD operations:

* **C**reate (add) with the `insert()` method.
* **R**ead (view) with the `getArgument()`, `findOption()` or `findValue()` methods.
* **U**pdate with the `replace()` method.
* **D**elete with the `remove()` method.



## Search for arguments ##

The `ArgumentList` class supports functions for searching through the arguments which can be used for basic argument validation.:

```cpp
#include <stdio.h>
#include <iostream>

#include "libargvcodec/ArgumentList.h"

int main(int argc, char **argv)
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
```



## Identify unknown arguments ##

To quickly identify unknown arguments, the `ArgumentList` class supports extract methods (one for each find method). Extract method removes the argument from the `ArgumentList` once a known argument is identified. Once all know arguments are removed from the list, the remaining ones are considered unknown.

```cpp
#include <stdio.h>
#include <iostream>

#include "libargvcodec/ArgumentList.h"

int main(int argc, char **argv)
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
```




# Build #

Please refer to file [INSTALL.md](INSTALL.md) for details on how installing/building the application.




# Platform #

libArgvCodec has been tested with the following platform:

*   Linux x86/x64
*   Windows x86/x64




# Versioning #

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/libArgvCodec/tags).




# Authors #

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/libArgvCodec/blob/master/AUTHORS) who participated in this project.




# License #

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
