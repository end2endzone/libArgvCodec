#include "TestCmdPromptArgumentCodec.h"
#include "libargvcodec/CmdPromptArgumentCodec.h"
#include "ArgumentLister.h"
#include "rapidassist/strings.h"
#include "TestUtils.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"

using namespace libargvcodec;

#define ASSERT_CSTR_EQ(val1, val2) ASSERT_EQ(std::string(val1), std::string(val2))

void TestCmdPromptArgumentCodec::SetUp()
{
}

void TestCmdPromptArgumentCodec::TearDown()
{
}

TEST_F(TestCmdPromptArgumentCodec, testDecodeCommandLine)
{
  const char * inputFile = "TestShellCommandLines.txt";

  ra::strings::StringVector cmdLines;
  ASSERT_TRUE( ra::gtesthelp::getTextFileContent(inputFile, cmdLines) );
  ASSERT_TRUE( cmdLines.size() > 0 );

  printf("\n");

  for(size_t i=0; i<cmdLines.size(); i++)
  {
    //arrange
    const std::string cmdLine = cmdLines[i];
    printf("Testing %d/%d: foo.exe %s\n", i+1, cmdLines.size(), cmdLine.c_str());

    //compute the expected list of arguments
    ArgumentList::StringList expectedArgs;
    bool success = systemDecodeCommandLineArguments(cmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success);

    //act
    CmdPromptArgumentCodec c;
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

TEST_F(TestCmdPromptArgumentCodec, testEncodeCommandLine)
{
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

    //arrange
    ra::strings::StringVector expectedArgs;
    ASSERT_TRUE( ra::gtesthelp::getTextFileContent(testFile.c_str(), expectedArgs) );
    printf("Testing %d/%d, %s\n", i+1, testFiles.size(), testFile.c_str());

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
    ASSERT_TRUE(success);

    //debug
    printf("  Expecting\n");
    for(size_t j=1; j<expectedArgs.size(); j++)
    {
      const std::string & arg = expectedArgs[j];
      printf("    argv[%d]=%s\n", j, arg.c_str() );
    }
    printf("  Actuals:\n");
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

    //next test file
  }
}

TEST_F(TestCmdPromptArgumentCodec, testEncodeCommandLine2)
{
  const char * inputFile = "TestShellCommandLines.txt";

  ra::strings::StringVector testCmdLines;
  ASSERT_TRUE( ra::gtesthelp::getTextFileContent(inputFile, testCmdLines) );
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
    CmdPromptArgumentCodec c;
    std::string cmdLine = c.encodeCommandLine(arglist);

    //compute the actual list of arguments
    ArgumentList::StringList actualArgs;
    success = systemDecodeCommandLineArguments(cmdLine.c_str(), actualArgs);
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

void prepareTestCmdPromptEncodeArgument(const char * iRawArguementValue, std::string & oEscapedArgument, std::string & oSystemArgumentValue)
{
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
}

TEST_F(TestCmdPromptArgumentCodec, testEncodeArgument)
{
  printf("\n");

  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "malicious argument\"&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\\"";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\ world";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test\\\"&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestCmdPromptEncodeArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------


}
