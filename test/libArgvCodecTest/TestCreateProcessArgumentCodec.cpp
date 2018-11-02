#include "TestCreateProcessArgumentCodec.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "libargvcodec/CreateProcessArgumentCodec.h"
#include "ArgumentLister.h"
#include "rapidassist/strings.h"
#include "rapidassist/gtesthelp.h"

using namespace libargvcodec;

void TestCreateProcessArgumentCodec::SetUp()
{
}

void TestCreateProcessArgumentCodec::TearDown()
{
}

TEST_F(TestCreateProcessArgumentCodec, testDecodeCommandLine)
{
#ifdef _WIN32 //requires CreateProcess() API
  const char * inputFile = "TestShellCommandLines.txt";

  ra::strings::StringVector cmdLines;
  ASSERT_TRUE( ra::gtesthelp::getTextFileContent(inputFile, cmdLines) );
  ASSERT_TRUE( cmdLines.size() > 0 );

  printf("\n");

  for(size_t i=0; i<cmdLines.size(); i++)
  {
    std::string message;

    //arrange
    const std::string cmdLine = cmdLines[i];
    message.append( ra::strings::format("Testing %d/%d: foo.exe %s\n", i+1, cmdLines.size(), cmdLine.c_str()) );
    printf("%s", message.c_str());

    //compute the expected list of arguments
    ArgumentList::StringList expectedArgs;
    bool success = createProcessDecodeCommandLineArguments(cmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success) << message;

    //act
    CreateProcessArgumentCodec c;
    ArgumentList actualArgs = c.decodeCommandLine(cmdLine.c_str());

    //building debug message in case of test failure
    message.append("  Expecting:\n");
    for(size_t i=1; i<expectedArgs.size(); i++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[i].c_str();
      message.append( ra::strings::format("   argv[%d]=%s\n", i, expectedArg) );
    }
    message.append("  Actuals:\n");
    for(int i=1; i<actualArgs.getArgc(); i++) //skip first argument since executable names may differ
    {
      const char * actualArg = actualArgs.getArgv()[i];
      message.append( ra::strings::format("   argv[%d]=%s\n", i, actualArg) );
    }

    //assert
    ASSERT_EQ( (int)expectedArgs.size(), actualArgs.getArgc() ) << message;
    //compare each argument
    for(int i=1; i<actualArgs.getArgc(); i++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[i].c_str();
      const char * actualArg = actualArgs.getArgv()[i];
      ASSERT_STREQ(expectedArg, actualArg) << message;
    }
  }
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
    const char * argumentValue      = "malicious argument\"&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_STREQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\\"";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_STREQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\ world";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_STREQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_STREQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test\\\"&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_STREQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------

#endif
}
