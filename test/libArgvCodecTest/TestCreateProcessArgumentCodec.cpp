#include "TestCreateProcessArgumentCodec.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "libargvcodec/CreateProcessArgumentCodec.h"
#include "ArgumentLister.h"
#include "TestUtils.h"
#include "rapidassist/strings.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"

using namespace libargvcodec;

void TestCreateProcessArgumentCodec::SetUp()
{
}

void TestCreateProcessArgumentCodec::TearDown()
{
}

bool hasCommandPromptShellCharacters(const std::string & iCmdLine)
{
  libargvcodec::CmdPromptArgumentCodec tmp;
  bool hasCmdPromptShellCharacters = tmp.hasShellCharacters(iCmdLine.c_str());
  return hasCmdPromptShellCharacters;
}

TEST_F(TestCreateProcessArgumentCodec, testDecodeCommandLine_testfile)
{
  libargvcodec::CreateProcessArgumentCodec codec;

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

    //Skip this test if it contains command prompt shell characters because CreateProcessArgumentCodec codec does not support shell characters.
    if (hasCommandPromptShellCharacters(file_cmdline))
    {
      //next series of test data
      continue;
    }

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
    bool system_success = getArgumentsFromCreateProcess(file_cmdline, system_arguments);
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

TEST_F(TestCreateProcessArgumentCodec, testEncodeCommandLine_testfile)
{
  libargvcodec::CreateProcessArgumentCodec codec;

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
    //On Windows system, also try to validate the generated command line with a CreateProcess() call.

    ra::strings::StringVector system_arguments;
    bool system_success = getArgumentsFromCreateProcess(generated_cmdline, system_arguments);
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

void prepareTestCreateProcessEncodeArgument(const char * iRawArguementValue, std::string & oEscapedArgument, std::string & oCreateProcessArgumentValue)
{
#ifdef _WIN32 //requires CreateProcess() API

  //arrange
  ArgumentList::StringList expectedArgs;
  expectedArgs.push_back("foo.exe");
  expectedArgs.push_back(iRawArguementValue);

  ArgumentList arglist;
  arglist.init(expectedArgs);

  //act
  CreateProcessArgumentCodec c;
  oEscapedArgument = c.encodeArgument(iRawArguementValue);
  oCreateProcessArgumentValue = createProcessDecodeArgument( oEscapedArgument.c_str() );

  //debug
  printf("Testing decoded argument argv[1]=%s\n", iRawArguementValue);
  printf("  Actual:\n");
  printf("    foo.exe %s\n", oEscapedArgument.c_str());
  printf("  Validating with CreateProcess() api...\n");
  printf("    argv[1]=%s\n", oCreateProcessArgumentValue.c_str());

#endif
}

TEST_F(TestCreateProcessArgumentCodec, testEncodeArgument)
{
#ifdef _WIN32 //requires CreateProcess() API

  printf("\n");

  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "malicious argument\"&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue.c_str(), actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_EQ(createProcessArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "\\\"hello\\\"";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue.c_str(), actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_EQ(createProcessArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "\\\"hello\\ world";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue.c_str(), actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_EQ(createProcessArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "test&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue.c_str(), actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_EQ(createProcessArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const std::string argumentValue      = "test\\\"&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue.c_str(), actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_EQ(createProcessArgumentValue, argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------

#endif
}
