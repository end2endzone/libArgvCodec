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

TEST_F(TestCreateProcessArgumentCodec, testDecodeCommandLine)
{
#ifndef _WIN32 //requires CreateProcess() API
  return;
#else
  std::string test_file = "Test.CommandLines.Windows.txt";
  ASSERT_TRUE( ra::filesystem::fileExists(test_file.c_str()) );

  //load file in memory
  ra::strings::StringVector content;
  bool readSuccess = ra::gtesthelp::getTextFileContent(test_file.c_str(), content);
  ASSERT_TRUE( readSuccess );

  libargvcodec::CreateProcessArgumentCodec codec;

  //process each command lines
  ra::strings::StringVector commands;
  for(size_t i=0; i<content.size(); i++)
  {
    //show progress for each 10% step
    static int previous_progress_percent = 0;
    int progress_percent = ((i+1)*100)/content.size();
    if (progress_percent % 10 == 0 && progress_percent > previous_progress_percent)
    {
      previous_progress_percent = progress_percent;
      printf("%d%%\n", progress_percent);
    }

    const std::string & line = content[i];
    if (isDashedLine(line))
    {
      //process current commands
      if (commands.size() >= 2) //must be at least 2 lines long (a cmdline and at least 1 argument)
      {
        //get command line and arguments from the file:
        std::string file_cmdline = commands[0];
        commands.erase(commands.begin());
        ra::strings::StringVector file_arguments = commands;

        //CreateProcessArgumentCodec codec does not support CmdPrompt shell characters
        bool hasCmdPromptShellCharacters = false;
        {
          libargvcodec::CmdPromptArgumentCodec tmp;
          hasCmdPromptShellCharacters = tmp.hasShellCharacters(file_cmdline.c_str());
        }
        if (hasCmdPromptShellCharacters)
        {
          //next series of commands
          commands.clear();
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

      //this command is completed
      commands.clear();
    }
    else
    {
      commands.push_back(line); //that is a command line string or an argument string
    }
  }

  //all the file is now processed.
#endif
}

TEST_F(TestCreateProcessArgumentCodec, testEncodeCommandLine)
{
#ifdef _WIN32 //requires CreateProcess() API
  const char * inputFile = "TestShellCommandLines.txt";

  ra::strings::StringVector testCmdLines;
  ASSERT_TRUE( ra::gtesthelp::getTextFileContent(inputFile, testCmdLines) );
  ASSERT_TRUE( testCmdLines.size() > 0 );

  printf("\n");

  //for each testCmdLines
  for(size_t i=0; i<testCmdLines.size(); i++)
  {
    std::string message;

    //arrange
    const std::string testCmdLine = testCmdLines[i];
    message.append( ra::strings::format("Testing %d/%d\n", i+1, testCmdLines.size()) );
    printf("%s", message.c_str());

    //compute the expected list of arguments
    ArgumentList::StringList expectedArgs;
    bool success = systemDecodeCommandLineArguments(testCmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success) << message;

    //build the list
    ArgumentList arglist;
    arglist.init(expectedArgs);

    //act
    CreateProcessArgumentCodec c;
    std::string cmdLine = c.encodeCommandLine(arglist);

    //compute the actual list of arguments
    ArgumentList::StringList actualArgs;
    success = createProcessDecodeCommandLineArguments(cmdLine.c_str(), actualArgs);
    ASSERT_TRUE(success) << message;

    //debug
    message.append("  Expecting\n");
    for(size_t j=1; j<expectedArgs.size(); j++)
    {
      const std::string & arg = expectedArgs[j];
      message.append( ra::strings::format("    argv[%d]=%s\n", j, arg.c_str()) );
    }
    message.append("  Actuals:\n");
    message.append( ra::strings::format("    cmdline=%s\n", cmdLine.c_str()) );
    for(size_t j=1; j<actualArgs.size(); j++)
    {
      const std::string & arg = actualArgs[j];
      message.append( ra::strings::format("    argv[%d]=%s\n", j, arg.c_str()) );
    }

    //assert
    ASSERT_EQ( expectedArgs.size(), actualArgs.size() ) << message;
    //compare each argument
    for(size_t j=1; j<expectedArgs.size(); j++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[j].c_str();
      const char * actualArg = actualArgs[j].c_str();
      ASSERT_STREQ(expectedArg, actualArg) << message;
    }
  }
#endif
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
