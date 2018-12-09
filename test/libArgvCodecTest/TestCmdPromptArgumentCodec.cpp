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

#include "TestCmdPromptArgumentCodec.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "rapidassist/strings.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/cppencoder.h"
#include "TestUtils.h"

using namespace libargvcodec;

void TestCmdPromptArgumentCodec::SetUp()
{
}

void TestCmdPromptArgumentCodec::TearDown()
{
}

TEST_F(TestCmdPromptArgumentCodec, testEncodeCommandLine_testfile)
{
  libargvcodec::CmdPromptArgumentCodec codec;

  std::string test_file = "Test.CommandLines.Windows.txt";

  TEST_DATA_LIST items;
  bool file_loaded = loadCommandLineTestFile(test_file, items);
  ASSERT_TRUE( file_loaded );

  for(size_t i=0; i<items.size(); i++)
  {
    const TEST_DATA & td = items[i];

    //show progress for each 10% step
    static int previous_progress_percent = 0;
    int progress_percent = ((i+1)*100)/items.size();
    if (progress_percent % 10 == 0 && progress_percent > previous_progress_percent)
    {
      previous_progress_percent = progress_percent;
      printf("%d%%\n", progress_percent);
    }

    //get command line and arguments from the file:
    const std::string & file_cmdline = td.cmdline;
    ra::strings::StringVector file_arguments = td.arguments;

    //build an ArgumentList
    ArgumentList arglist;
    {
      ra::strings::StringVector tmp_arguments = file_arguments;

      //add a fake executable
      #ifdef _WIN32
      tmp_arguments.insert(tmp_arguments.begin(), "showargs.exe");
      #elif defined(__linux__)
      tmp_arguments.insert(tmp_arguments.begin(), "./showargs");
      #endif

      //initialize the ArgumentList class
      arglist.init(tmp_arguments);
    }

    //encode the arguments into a command line
    std::string generated_cmdline = codec.encodeCommandLine(arglist);

    //decode the generated command line
    ra::strings::StringVector actual_arguments = toStringList(codec.decodeCommandLine(generated_cmdline.c_str()));

    //build a meaningful error message
    std::string error_message = buildErrorString(file_arguments, generated_cmdline, actual_arguments);

    //assert actual arguments are equals to expected arguments
    ASSERT_EQ(file_arguments.size(), actual_arguments.size()) << error_message;
    for(size_t j=0; j<file_arguments.size(); j++)
    {
      const std::string & expected_argument = file_arguments[j];
      const std::string & actual_argument   = actual_arguments[j];
      ASSERT_EQ(expected_argument, actual_argument) << error_message;
    }

#ifdef _WIN32
    //On Windows system, also try to validate the generated command line with a system() call.

    ra::strings::StringVector system_arguments;
    bool system_success = getArgumentsFromSystem(generated_cmdline, system_arguments);
    ASSERT_TRUE( system_success );

    //build a meaningful error message
    error_message = buildErrorString(file_arguments, generated_cmdline, system_arguments);

    //assert codec arguments are equals to file arguments
    ASSERT_EQ(file_arguments.size(), system_arguments.size()) << error_message;
    for(size_t j=0; j<file_arguments.size(); j++)
    {
      const std::string & expected_argument = file_arguments[j];
      const std::string & system_argument   = system_arguments[j];
      ASSERT_EQ(expected_argument, system_argument) << error_message;
    }
#endif //_WIN32
  }
}

std::string getFirstArgumentFromSystem(const char * iValue)
{
  ra::strings::StringVector arguments;
  bool success = getArgumentsFromSystem(iValue, arguments);
  if (!success)
    return "";
  if (arguments.size() != 1)
    return "";

  std::string confirmedArgumentValue = arguments[0];
  return confirmedArgumentValue;
}

void prepareTestCmdPromptEncodeArgument(const char * iRawArguementValue, std::string & oEscapedArgument, std::string & oSystemArgumentValue)
{
#ifdef _WIN32
  //arrange
  ArgumentList::StringList expectedArgs;
  expectedArgs.push_back("foo.exe");
  expectedArgs.push_back(iRawArguementValue);

  ArgumentList arglist;
  arglist.init(expectedArgs);

  //act
  CmdPromptArgumentCodec c;
  oEscapedArgument = c.encodeArgument(iRawArguementValue);
  oSystemArgumentValue = getFirstArgumentFromSystem( oEscapedArgument.c_str() );

  //debug
  printf("Testing decoded argument argv[1]=%s\n", iRawArguementValue);
  printf("  Actual:\n");
  printf("    foo.exe %s\n", oEscapedArgument.c_str());
  printf("  Validating with system's cmd.exe...\n");
  printf("    argv[1]=%s\n", oSystemArgumentValue.c_str());
#endif
}

TEST_F(TestCmdPromptArgumentCodec, testEncodeArgument)
{
#ifdef _WIN32
  printf("\n");

  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "malicious argument\"&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue.c_str(), actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_EQ(systemArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "\\\"hello\\\"";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue.c_str(), actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_EQ(systemArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "\\\"hello\\ world";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue.c_str(), actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_EQ(systemArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "test&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue.c_str(), actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_EQ(systemArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "test\\\"&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue.c_str(), actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_EQ(systemArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------

#endif
}

TEST_F(TestCmdPromptArgumentCodec, testDecodeArgument_testfile)
{
  //The objective of this unit test is to validate the content of file 'Test.CommandLines.Windows.txt' with CmdPromptArgumentCodec::decodeCommandLine() implementation.
  //On Windows systems, the content of file 'Test.CommandLines.Windows.txt' is also validated with a system() call.

  libargvcodec::CmdPromptArgumentCodec codec;

  std::string test_file = "Test.CommandLines.Windows.txt";

  TEST_DATA_LIST items;
  bool file_loaded = loadCommandLineTestFile(test_file, items);
  ASSERT_TRUE( file_loaded );

  for(size_t i=0; i<items.size(); i++)
  {
    const TEST_DATA & td = items[i];

    //show progress for each 10% step
    static int previous_progress_percent = 0;
    int progress_percent = ((i+1)*100)/items.size();
    if (progress_percent % 10 == 0 && progress_percent > previous_progress_percent)
    {
      previous_progress_percent = progress_percent;
      printf("%d%%\n", progress_percent);
    }

    //get command line and arguments from the file:
    const std::string & file_cmdline = td.cmdline;
    const ra::strings::StringVector & file_arguments = td.arguments;

    //compare againts codec
    ra::strings::StringVector codec_arguments = toStringList(codec.decodeCommandLine(file_cmdline.c_str()));

    //build a meaningful error message
    std::string error_message = buildErrorString(file_cmdline, file_arguments, codec_arguments);

    //assert codec arguments are equals to file arguments
    ASSERT_EQ(file_arguments.size(), codec_arguments.size()) << error_message;
    for(size_t j=0; j<file_arguments.size(); j++)
    {
      const std::string & expected_argument = file_arguments[j];
      const std::string & codec_argument   = codec_arguments[j];
      ASSERT_EQ(expected_argument, codec_argument) << error_message;
    }

#ifdef _WIN32
    //On Windows system, also try to validate content of test file againts a system() call.

    ra::strings::StringVector system_arguments;
    bool system_success = getArgumentsFromSystem(file_cmdline, system_arguments);
    ASSERT_TRUE( system_success );

    //build a meaningful error message
    error_message = buildErrorString(file_cmdline, file_arguments, system_arguments);

    //assert codec arguments are equals to file arguments
    ASSERT_EQ(file_arguments.size(), system_arguments.size()) << error_message;
    for(size_t j=0; j<file_arguments.size(); j++)
    {
      const std::string & expected_argument = file_arguments[j];
      const std::string & system_argument   = system_arguments[j];
      ASSERT_EQ(expected_argument, system_argument) << error_message;
    }
#endif //_WIN32

  }
}

TEST_F(TestCmdPromptArgumentCodec, testPrintableCharacters)
{
  libargvcodec::CmdPromptArgumentCodec codec;
 
  //build the list of test items
  TEST_DATA_LIST items;
  for(int i=-128; i<=127; i++)
  {
    char c = (char)i;
    char cStr[] = {c, 0};
 
    if (!ra::cppencoder::isPrintableCharacter(c))
      continue;
    if (c == '\n') //not a supported argument character
      continue;
    if (c == '\r') //not a supported argument character
      continue;
 
    //create test data
    TEST_DATA td;
    td.cmdline = codec.encodeArgument(cStr); //encode with codec
    td.arguments.push_back( std::string(cStr) );
    items.push_back(td);
  }
 
  //test each items
  for(size_t i=0; i<items.size(); i++)
  {
    const TEST_DATA & td = items[i];
 
    //show progress for each 10% step
    static int previous_progress_percent = 0;
    int progress_percent = ((i+1)*100)/items.size();
    if (progress_percent % 10 == 0 && progress_percent > previous_progress_percent)
    {
      previous_progress_percent = progress_percent;
      printf("%d%%\n", progress_percent);
    }
 
    //get command line and arguments from the file:
    const std::string & test_cmdline = td.cmdline;
    const ra::strings::StringVector & test_arguments = td.arguments;
 
    //compare againts codec
    ra::strings::StringVector codec_arguments = toStringList(codec.decodeCommandLine(test_cmdline.c_str()));
 
    //build a meaningful error message
    std::string error_message = buildErrorString(test_cmdline, test_arguments, codec_arguments);
 
    //assert codec arguments are equals to file arguments
    ASSERT_EQ(test_arguments.size(), codec_arguments.size()) << error_message;
    for(size_t j=0; j<test_arguments.size(); j++)
    {
      const std::string & expected_argument = test_arguments[j];
      const std::string & codec_argument   = codec_arguments[j];
      ASSERT_EQ(expected_argument, codec_argument) << error_message;
    }
 
#ifdef _WIN32
    //On Windows system, also try to validate content of test file againts a system() call.
 
    ra::strings::StringVector system_arguments;
    bool system_success = getArgumentsFromSystem(test_cmdline, system_arguments);
    ASSERT_TRUE( system_success );
 
    //build a meaningful error message
    error_message = buildErrorString(test_cmdline, test_arguments, system_arguments);
 
    //assert codec arguments are equals to file arguments
    ASSERT_EQ(test_arguments.size(), system_arguments.size()) << error_message;
    for(size_t j=0; j<test_arguments.size(); j++)
    {
      const std::string & expected_argument = test_arguments[j];
      const std::string & system_argument   = system_arguments[j];
      ASSERT_EQ(expected_argument, system_argument) << error_message;
    }
#endif // _WIN32
  }
}
