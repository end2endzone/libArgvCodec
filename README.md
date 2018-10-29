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
This library tries to solve a decade old problem: how to properly encode command line arguments. Properly encoding string arguments into a long command line should not be a complicated issue. However, I have seen multiple situations where people wrongfully assume that wrapping each argument with double quotes is "safe enough" for most situations. That is simply not true.

The purpose of the library is not to handle *Argument Parsing* and *Argument Validation* : there are already multiple libraries that can do that and most of them does it pretty well.

However, few of them are actually developed to help a user passing arguments from one application to another. The libArgvCodec library includes arguments and command line encoder and decoder (codecs) that are designed for this particular purpose.

Transferring local variable values to another application through command line arguments is not something that occurs pretty often but if you ever have to deal with this situation, this library can help you avoid typical encoding pitfalls.




# Features #

The main features of the library are:

* Supports `int argc, char* argv[]` argument initialization.
* Supports case sensitive and case insensitive searches within lists of arguments.
* Quickly identify unknown arguments.
* Supports any type of argument prefixes like: slashes, single dash, double dash, etc.
* Encodes multiple single arguments into a full command line.
* Decodes a command line into individual arguments.
* Supports both Windows shell (command prompt) and Windows [CreateProcess()](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682425(v=vs.85).aspx) encoding/decoding.
* Bulletproof application againts command line injection attacks.

Limitations:
* The library can be compiled in Linux. However, the current encoders/decoders does not support Linux terminal parameters.




# Rules #

Please refer to file [Rules.Windows.md](Rules.Windows.md) for the list of rules implemented into the library for properly encoding arguments into a command line or parsing a command line.




# Usage #

The following instructions show how to use the library.



## Arguments validation ##

Parsing the list of arguments for validation is easy with the libArgvCodec. Create an instance of the `ArgumentList` class which encapsulates a list of argument values. Call the `init()` method using the application’s `argc` and `argv` parameters which initialize the instance with a copy of all the parameters.

Browse through the list of arguments using the `getArgument()` method or search for a specific parameter using the `findIndex()` or `contains()` methods.

Searching for **options** (argument of the type `/p` or `–help` or `start`) is easily done with the class using the `findOption()` method.

Search for arguments with values for patterns likes `/name=foo` or `–count=5`) using the `findValue()` method.

Search for **name and value pairs** (arguments whose value is following as another argument) for patterns like `/product foo` or `–repeat 5` using the `findNextValue()` method.

To quickly identify unknown arguments, the class supports extract methods (one for each find method). Extract method removes the argument from the `ArgumentList`. Once all know arguments are removed from the list, the remaining ones are considered unknown.



## Manipulating an argument list ##

The `ArgumentList` class is used as a container for arguments. It supports all CRUD operations:

* **C**reate (add) with the `insert()` method.
* **R**ead (view) with the `getArgument()` method.
* **U**pdate with the `replace()` method.
* **D**elete with the `remove()` method.



## Encoding arguments and decoding command line ##

The encoding and decoding of command line arguments is with `IArgumentEncoder` and `IArgumentDecoder` interfaces. To interact with the interfaces, the `ArgumentList` class is used as input data or output data.

The library provides the following classes which implements both interfaces:

* `CmdPromptArgumentCodec` handles encoding/decoding for the Windows command prompt (shell).
* `CreateProcessArgumentCodec` handles encoding/decoding for the Windows [CreateProcess()](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682425(v=vs.85).aspx) api.

Using these classes, one can fill up an ArgumentList instance and call the encodeCommandLine() method to encode all the arguments in the list into a single command line.

Using the same codec classes, one can "split" a command line into multiple arguments using the `decodeCommandLine()` method which returns a valid `ArgumentList` with the result.



## Source code example ##

The following section shows multiple examples of using libArgvCodec.

### Encoding values into a command line ###

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
  arglist.insert("foo.exe"); //argv[0] is always the path to the current process

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
