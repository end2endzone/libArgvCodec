#include "TestCreateProcessArgumentCodec.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "libargvcodec/CreateProcessArgumentCodec.h"
#include "ArgumentLister.h"
#include "rapidassist/strings.h"
#include "gTestHelper.h"

using namespace libargvcodec;

#define ASSERT_CSTR_EQ(val1, val2) ASSERT_EQ(std::string(val1), std::string(val2))

void TestCreateProcessArgumentCodec::SetUp()
{
}

void TestCreateProcessArgumentCodec::TearDown()
{
}

TEST_F(TestCreateProcessArgumentCodec, testDecodeCommandLine)
{
  const char * inputFile = "shellCommandLines.txt";

  gTestHelper::StringVector cmdLines;
  ASSERT_TRUE( gTestHelper::getInstance().getTextFileContent(inputFile, cmdLines) );
  ASSERT_TRUE( cmdLines.size() > 0 );

  printf("\n");

  for(size_t i=0; i<cmdLines.size(); i++)
  {
    //arrange
    const std::string cmdLine = cmdLines[i];
    printf("Testing %d/%d: foo.exe %s\n", i+1, cmdLines.size(), cmdLine.c_str());

    //compute the expected list of arguments
    ArgumentList::StringList expectedArgs;
    bool success = createProcessDecodeCommandLineArguments(cmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success);

    //act
    CreateProcessArgumentCodec c;
    ArgumentList actualArgs = c.decodeCommandLine(cmdLine.c_str());

    //debug
    printf("  Expecting:\n");
    for(size_t i=1; i<expectedArgs.size(); i++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[i].c_str();
      printf("   argv[%d]=%s\n", i, expectedArg);
    }
    printf("  Actuals:\n");
    for(int i=1; i<actualArgs.getArgc(); i++) //skip first argument since executable names may differ
    {
      const char * actualArg = actualArgs.getArgv()[i];
      printf("   argv[%d]=%s\n", i, actualArg);
    }

    //assert
    ASSERT_EQ( (int)expectedArgs.size(), actualArgs.getArgc() );
    //compare each argument
    for(int i=1; i<actualArgs.getArgc(); i++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[i].c_str();
      const char * actualArg = actualArgs.getArgv()[i];
      ASSERT_CSTR_EQ(expectedArg, actualArg);
    }
    printf("  success\n");
  }
}

TEST_F(TestCreateProcessArgumentCodec, testEncodeCommandLine)
{
  const char * inputFile = "shellCommandLines.txt";

  gTestHelper::StringVector testCmdLines;
  ASSERT_TRUE( gTestHelper::getInstance().getTextFileContent(inputFile, testCmdLines) );
  ASSERT_TRUE( testCmdLines.size() > 0 );

  printf("\n");

  //for each testCmdLines
  for(size_t i=0; i<testCmdLines.size(); i++)
  {
    //arrange
    const std::string testCmdLine = testCmdLines[i];
    printf("Testing %d/%d\n", i+1, testCmdLines.size());

    //compute the expected list of arguments
    ArgumentList::StringList expectedArgs;
    bool success = systemDecodeCommandLineArguments(testCmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success);

    //build the list
    ArgumentList arglist;
    arglist.init(expectedArgs);

    //act
    CreateProcessArgumentCodec c;
    std::string cmdLine = c.encodeCommandLine(arglist);

    //compute the actual list of arguments
    ArgumentList::StringList actualArgs;
    success = createProcessDecodeCommandLineArguments(cmdLine.c_str(), actualArgs);
    ASSERT_TRUE(success);

    //debug
    printf("  Expecting\n");
    for(size_t j=1; j<expectedArgs.size(); j++)
    {
      const std::string & arg = expectedArgs[j];
      printf("    argv[%d]=%s\n", j, arg.c_str() );
    }
    printf("  Actuals:\n");
    printf("    cmdline=%s\n", cmdLine.c_str());
    for(size_t j=1; j<actualArgs.size(); j++)
    {
      const std::string & arg = actualArgs[j];
      printf("    argv[%d]=%s\n", j, arg.c_str() );
    }

    //assert
    ASSERT_EQ( expectedArgs.size(), actualArgs.size() );
    //compare each argument
    for(size_t j=1; j<expectedArgs.size(); j++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[j].c_str();
      const char * actualArg = actualArgs[j].c_str();
      ASSERT_CSTR_EQ(expectedArg, actualArg);
    }
    printf("  success\n");
  }
}

void prepareTestCreateProcessEncodeArgument(const char * iRawArguementValue, std::string & oEscapedArgument, std::string & oCreateProcessArgumentValue)
{
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
}

TEST_F(TestCreateProcessArgumentCodec, testEncodeArgument)
{
  printf("\n");

  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "malicious argument\"&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_CSTR_EQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\\"";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_CSTR_EQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\ world";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_CSTR_EQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_CSTR_EQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test\\\"&whoami";
    std::string    actualEscapedArg;
    std::string createProcessArgumentValue;

    prepareTestCreateProcessEncodeArgument(argumentValue, actualEscapedArg, createProcessArgumentValue);

    //assert
    ASSERT_CSTR_EQ(createProcessArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------


}
