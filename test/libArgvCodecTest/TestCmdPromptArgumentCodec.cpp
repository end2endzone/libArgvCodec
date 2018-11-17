#include "TestCmdPromptArgumentCodec.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "ArgumentLister.h"
#include "rapidassist/strings.h"
#include "TestUtils.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"

using namespace libargvcodec;

void TestCmdPromptArgumentCodec::SetUp()
{
}

void TestCmdPromptArgumentCodec::TearDown()
{
}

TEST_F(TestCmdPromptArgumentCodec, testEncodeCommandLine)
{
#ifdef _WIN32
  const char * inputFilePrefix = "TestEncodeCommandLine";
  const char * inputFilePostfix = ".txt";
  
  //discover test files
  ArgumentList::StringList testFiles;
  static const int MAX_ID_LENGTH = 3;
  int fileId = 1;
  std::string inputFile = getSequencedFile(inputFilePrefix, fileId, inputFilePostfix, MAX_ID_LENGTH);
  bool fileFound = ra::filesystem::fileExists( inputFile.c_str() );
  while(fileFound)
  {
    testFiles.push_back(inputFile);

    //prepare for next loop
    fileId++;
    inputFile = getSequencedFile(inputFilePrefix, fileId, inputFilePostfix, MAX_ID_LENGTH);
    fileFound = ra::filesystem::fileExists( inputFile.c_str() );
  }
  ASSERT_TRUE( testFiles.size() > 0 );

  printf("\n");

  //for each test files
  for(size_t i=0; i<testFiles.size(); i++)
  {
    const std::string & testFile = testFiles[i];

    std::string message;

    //arrange
    ra::strings::StringVector expectedArgs;
    ASSERT_TRUE( ra::gtesthelp::getTextFileContent(testFile.c_str(), expectedArgs) );
    message.append( ra::strings::format("Testing %d/%d, %s\n", i+1, (int)testFiles.size(), testFile.c_str()) );
    printf("%s", message.c_str());

    //insert fake .exe name
    expectedArgs.insert(expectedArgs.begin(), "foo.exe");

    ArgumentList arglist;
    arglist.init(expectedArgs);

    //act
    CmdPromptArgumentCodec c;
    std::string cmdLine = c.encodeCommandLine(arglist);

    //decode the generated command line using the system's perspective
    ArgumentList::StringList actualArgs;
    bool success = systemDecodeCommandLineArguments(cmdLine.c_str(), actualArgs);
    ASSERT_TRUE(success) << message.c_str();

    //building debug message in case of test failure
    message.append("  Expecting\n");
    for(size_t j=1; j<expectedArgs.size(); j++)
    {
      const std::string & arg = expectedArgs[j];
      message.append( ra::strings::format("    argv[%d]=%s\n", (int)j, arg.c_str()).c_str() );
    }
    message.append("  Actuals:\n");
    for(size_t j=1; j<actualArgs.size(); j++)
    {
      const std::string & arg = actualArgs[j];
      message.append( ra::strings::format("    argv[%d]=%s\n", (int)j, arg.c_str()).c_str() );
    }

    //assert
    ASSERT_EQ( expectedArgs.size(), actualArgs.size() ) << message.c_str();
    //compare each argument
    for(size_t j=1; j<expectedArgs.size(); j++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[j].c_str();
      const char * actualArg = actualArgs[j].c_str();
      ASSERT_STREQ(expectedArg, actualArg) << message.c_str();
    }

    //next test file
  }
#endif
}

TEST_F(TestCmdPromptArgumentCodec, testEncodeCommandLine2)
{
#ifdef _WIN32
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
    message.append( ra::strings::format("Testing %d/%d\n", (int)(i+1), testCmdLines.size()) );
    printf("%s", message.c_str());

    //compute the expected list of arguments
    ArgumentList::StringList expectedArgs;
    bool success = systemDecodeCommandLineArguments(testCmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success) << message.c_str();

    //build the list
    ArgumentList arglist;
    arglist.init(expectedArgs);

    //act
    CmdPromptArgumentCodec c;
    std::string cmdLine = c.encodeCommandLine(arglist);

    //compute the actual list of arguments
    ArgumentList::StringList actualArgs;
    success = systemDecodeCommandLineArguments(cmdLine.c_str(), actualArgs);
    ASSERT_TRUE(success) << message.c_str();

    //building debug message in case of test failure
    message.append("  Expecting\n");
    for(size_t j=1; j<expectedArgs.size(); j++)
    {
      const std::string & arg = expectedArgs[j];
      message.append( ra::strings::format("    argv[%d]=%s\n", (int)j, arg.c_str()) );
    }
    message.append("  Actuals:\n");
    message.append( ra::strings::format("    cmdline=%s\n", cmdLine.c_str()) );
    for(size_t j=1; j<actualArgs.size(); j++)
    {
      const std::string & arg = actualArgs[j];
      message.append( ra::strings::format("    argv[%d]=%s\n", (int)j, arg.c_str()) );
    }

    //assert
    ASSERT_EQ( expectedArgs.size(), actualArgs.size() ) << message.c_str();
    //compare each argument
    for(size_t j=1; j<expectedArgs.size(); j++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[j].c_str();
      const char * actualArg = actualArgs[j].c_str();
      ASSERT_STREQ(expectedArg, actualArg) << message.c_str();
    }
  }
#endif
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
  oSystemArgumentValue = systemDecodeArgument( oEscapedArgument.c_str() );

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

TEST_F(TestCmdPromptArgumentCodec, testSystem)
{
  //The objective of this unit test is to validate the content of file 'Test.CommandLines.Windows.txt' with CmdPromptArgumentCodec::decodeCommandLine() implementation.
  //On Windows systems, the content of file 'Test.CommandLines.Windows.txt' is also validated with a system() call.

  std::string test_file = "Test.CommandLines.Windows.txt";
  ASSERT_TRUE( ra::filesystem::fileExists(test_file.c_str()) );

  //load file in memory
  ra::strings::StringVector content;
  bool readSuccess = ra::gtesthelp::getTextFileContent(test_file.c_str(), content);
  ASSERT_TRUE( readSuccess );

  libargvcodec::CmdPromptArgumentCodec codec;

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

        //compare againts codec
        ra::strings::StringVector codec_arguments = toStringList(codec.decodeCommandLine(file_cmdline.c_str()));

        //build a meaningful error message
        std::string error_message;
        error_message += "The command line `" + file_cmdline + "`\n";
        error_message += "was expected to return the following arguments:\n";
        for(size_t j=0; j<file_arguments.size(); j++)
        {
          error_message += "  arg[" + ra::strings::toString(j) + "]: ";
          error_message += file_arguments[j] + "\n";
        }
        error_message += "but decoding the command line actually returned the following arguments:\n";
        for(size_t j=0; j<codec_arguments.size(); j++)
        {
          error_message += "  arg[" + ra::strings::toString(j) + "]: ";
          error_message += codec_arguments[j] + "\n";
        }

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
        error_message = "";
        error_message += "The command line `" + file_cmdline + "`\n";
        error_message += "was expected to return the following arguments:\n";
        for(size_t j=0; j<file_arguments.size(); j++)
        {
          error_message += "  arg[" + ra::strings::toString(j) + "]: ";
          error_message += file_arguments[j] + "\n";
        }
        error_message += "but system() actually returned the following arguments:\n";
        for(size_t j=0; j<system_arguments.size(); j++)
        {
          error_message += "  arg[" + ra::strings::toString(j) + "]: ";
          error_message += system_arguments[j] + "\n";
        }

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
}
