#include "gtest_emulator.h"
#include "ArgumentManager.h"
#include "ArgumentLister.h"
#include "utils.h"

#define ASSERT_CSTR_EQ(val1, val2) ASSERT_EQ(std::string(val1), std::string(val2))

std::string getSequencedFile(const char * iPrefix, int iValue, const char * iPostfix, int iValueLength)
{
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];

  char valueFormat[BUFFER_SIZE];
  char value[BUFFER_SIZE];
  sprintf(valueFormat, "%%0%dd", iValueLength);
  sprintf(value, valueFormat, iValue);

  sprintf(buffer, "%s%s%s", iPrefix, value, iPostfix);

  return std::string(buffer);
}

bool isIdentical(ArgumentManager & m, int expectedArgc, char ** expectedArgv)
{
  int mArgc = m.getArgc();
  char** mArgv = m.getArgv();
  
  if (mArgc != expectedArgc)
    return false;

  for(int i=0; i<mArgc; i++)
  {
    const char * mValue1 = m.getArgument(i);
    const char * mValue2 = mArgv[i];

    //should be same pointer
    if (mValue1 != mValue2)
      return false;

    //mValue1 and expectedArgv contents should be identical
    if (std::string(mValue1) != std::string(expectedArgv[i]))
      return false;
  }

  //check last position for NULL string
  if (expectedArgv[expectedArgc] != NULL)
    return false;
  if (mArgv[expectedArgc] != NULL)
    return false;

  return true;
}

TEST_F(ArgumentManager, foo)
{
  ArgumentManager m;
  ////m.init("a ^\"^\" b");
  //m.init("^\"^\" a b");
  //m.init("a b ^\"^\"");

////Testing 100/105: foo.exe ^"test\"^&whoami^"
//  std::string inputArg = "^\"test\\\"^&whoami^\"";
//  m.init(inputArg.c_str());
//  std::string encodedArg = m.getCommandLineArgument(1);
//  std::string actualArg = m.getArgv()[1];
//  std::string expectedArg = "test\"^&whoami^";
//  Expecting:
//   argv[1]=test"^&whoami^
//  Actuals:
//   argv[1]=test"&whoami



////Testing 78/110
//  std::string arg1 = "ab cd\\^&echo";
//  std::string arg2 = "foo";
//  char * argv[] = {(char*)"foo.exe", (char*)arg1.c_str(), (char*)arg2.c_str()};
//  int argc = sizeof(argv)/sizeof(argv[0]);
//  m.init(argc, argv);
//  std::string actualCmdLine = m.getCommandLine();
//  std::string expectedCmdLine = "";  
////  Expecting
////    argv[1]=ab cd\^&echo
////    argv[2]=foo
////  Actuals:
////    cmdline=^"ab cd\^^\^&\echo^" foo
////    argv[1]=ab cd\^\&\echo
////    argv[2]=foo


////Testing 21/110
//  char * argv[] = { (char*)"foo.exe",
//                    (char*)"a",
//                    (char*)"b\\\\",
//                    (char*)"\"",
//                    (char*)"c",
//                    (char*)"d",
//  };
//  int argc = sizeof(argv)/sizeof(argv[0]);
//  m.init(argc, argv);
//  std::string actualCmdLine = m.getCommandLine();
////  Expecting
////    argv[1]=a
////    argv[2]=b\\
////    argv[3]="
////    argv[4]=c
////    argv[5]=d
////  Actuals:
////    cmdline=a b\\\\ \" c d
////    argv[1]=a
////    argv[2]=b\\\\
////    argv[3]="
////    argv[4]=c
////    argv[5]=d


////Testing 49/110
////forces\beauchamp.a3
//  char * argv[] = { (char*)"foo.exe",
//                    (char*)"test\"&whoami",
//  };
//  int argc = sizeof(argv)/sizeof(argv[0]);
//  m.init(argc, argv);
//  std::string actualCmdLine = m.getCommandLine();
////  Expecting
////    argv[1]=test"&whoami
////  Actuals:
////    cmdline="test\"&whoami"
////    argv[1]=test"

  ASSERT_FALSE(false);
}

TEST_F(ArgumentManager, testInitArgcArgv)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);
  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc, argc2);
  ASSERT_CSTR_EQ(argv[0], argv2[0]);
  ASSERT_CSTR_EQ(argv[1], argv2[1]);
  ASSERT_CSTR_EQ(argv[2], argv2[2]);
  ASSERT_CSTR_EQ(argv[3], argv2[3]);
  ASSERT_EQ(argv[4], argv2[4]);
}

TEST_F(ArgumentManager, testGetArgument)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);
  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc, argc2);
  ASSERT_CSTR_EQ(argv[0], m.getArgument(0));
  ASSERT_CSTR_EQ(argv[1], m.getArgument(1));
  ASSERT_CSTR_EQ(argv[2], m.getArgument(2));
  ASSERT_CSTR_EQ(argv[3], m.getArgument(3));
  ASSERT_CSTR_EQ("", m.getArgument(4)); //out of bounds
  ASSERT_CSTR_EQ("", m.getArgument(5)); //out of bounds
  ASSERT_CSTR_EQ("", m.getArgument(6)); //out of bounds
}

TEST_F(ArgumentManager, testInitVector)
{
  std::vector<std::string> args;
  args.push_back("test.exe");
  args.push_back("/p");
  args.push_back("-logfile=log.txt");
  args.push_back("count=5");
  int argc = (int)args.size();

  ArgumentManager m;
  m.init(args);
  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc, argc2);
  ASSERT_EQ(args[0], std::string(argv2[0]));
  ASSERT_EQ(args[1], std::string(argv2[1]));
  ASSERT_EQ(args[2], std::string(argv2[2]));
  ASSERT_EQ(args[3], std::string(argv2[3]));
  ASSERT_EQ(NULL, argv2[4]);
}


TEST_F(ArgumentManager, DISABLED_testInitString)
{
  const char * inputFile = "testInitString.txt";

  ArgumentManager::StringList cmdLines;
  ASSERT_TRUE( utils::readTextFile(inputFile, cmdLines) );
  ASSERT_TRUE( cmdLines.size() > 0 );

  printf("\n");

  for(size_t i=0; i<cmdLines.size(); i++)
  {
    //arrange
    const std::string cmdLine = cmdLines[i];
    printf("Testing %d/%d: foo.exe %s\n", i+1, cmdLines.size(), cmdLine.c_str());

    //compute the expected list of arguments
    ArgumentManager::StringList expectedArgs;
    bool success = decodeCommandLineArguments(cmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success);

    //act
    ArgumentManager mgr;
    mgr.init(cmdLine.c_str());

    //debug
    printf("  Expecting:\n");
    for(size_t i=1; i<expectedArgs.size(); i++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[i].c_str();
      printf("   argv[%d]=%s\n", i, expectedArg);
    }
    printf("  Actuals:\n");
    for(int i=1; i<mgr.getArgc(); i++) //skip first argument since executable names may differ
    {
      const char * actualArg = mgr.getArgv()[i];
      printf("   argv[%d]=%s\n", i, actualArg);
    }

    //assert
    ASSERT_EQ( (int)expectedArgs.size(), mgr.getArgc() );
    //compare each argument
    for(int i=1; i<mgr.getArgc(); i++) //skip first argument since executable names may differ
    {
      const char * expectedArg = expectedArgs[i].c_str();
      const char * actualArg = mgr.getArgv()[i];
      ASSERT_CSTR_EQ(expectedArg, actualArg);
    }
    printf("  success\n");
  }
}

TEST_F(ArgumentManager, testGetCommandLine)
{
  const char * inputFilePrefix = "testGetCommandLine";
  const char * inputFilePostfix = ".txt";
  
  //discover test files
  ArgumentManager::StringList testFiles;
  static const int MAX_ID_LENGTH = 3;
  int fileId = 1;
  std::string inputFile = getSequencedFile(inputFilePrefix, fileId, inputFilePostfix, MAX_ID_LENGTH);
  bool fileFound = utils::fileExists( inputFile.c_str() );
  while(fileFound)
  {
    testFiles.push_back(inputFile);

    //prepare for next loop
    fileId++;
    inputFile = getSequencedFile(inputFilePrefix, fileId, inputFilePostfix, MAX_ID_LENGTH);
    fileFound = utils::fileExists( inputFile.c_str() );
  }
  ASSERT_TRUE( testFiles.size() > 0 );

  printf("\n");

  //for each test files
  for(size_t i=0; i<testFiles.size(); i++)
  {
    const std::string & testFile = testFiles[i];

    //arrange
    ArgumentManager::StringList expectedArgs;
    ASSERT_TRUE( utils::readTextFile(testFile.c_str(), expectedArgs) );
    printf("Testing %d/%d, %s\n", i+1, testFiles.size(), testFile.c_str());

    //insert fake .exe name
    expectedArgs.insert(expectedArgs.begin(), "foo.exe");

    ArgumentManager mgr;
    mgr.init(expectedArgs);

    //act
    std::string cmdLine = mgr.getCommandLine();

    //compute the actual list of arguments
    ArgumentManager::StringList actualArgs;
    bool success = decodeCommandLineArguments(cmdLine.c_str(), actualArgs);
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

TEST_F(ArgumentManager, testGetCommandLine2)
{
  const char * inputFile = "testInitString.txt";

  ArgumentManager::StringList testCmdLines;
  ASSERT_TRUE( utils::readTextFile(inputFile, testCmdLines) );
  ASSERT_TRUE( testCmdLines.size() > 0 );

  printf("\n");

  //for each testCmdLines
  for(size_t i=0; i<testCmdLines.size(); i++)
  {
    //arrange
    const std::string testCmdLine = testCmdLines[i];
    printf("Testing %d/%d\n", i+1, testCmdLines.size());

    //compute the expected list of arguments
    ArgumentManager::StringList expectedArgs;
    bool success = decodeCommandLineArguments(testCmdLine.c_str(), expectedArgs);
    ASSERT_TRUE(success);

    //init the manager
    ArgumentManager mgr;
    mgr.init(expectedArgs);

    //act
    std::string cmdLine = mgr.getCommandLine();

    //compute the actual list of arguments
    ArgumentManager::StringList actualArgs;
    success = decodeCommandLineArguments(cmdLine.c_str(), actualArgs);
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

void prepareTestGetCommandLineArgument(const char * iRawArguementValue, std::string & oEscapedArgument, std::string & oSystemArgumentValue)
{
  //arrange
  ArgumentManager::StringList expectedArgs;
  expectedArgs.push_back("foo.exe");
  expectedArgs.push_back(iRawArguementValue);

  ArgumentManager mgr;
  mgr.init(expectedArgs);

  //act
  oEscapedArgument = mgr.getCommandLineArgument(1);
  oSystemArgumentValue = decodeArgument( oEscapedArgument.c_str() );

  //debug
  printf("Testing decoded argument argv[1]=%s\n", iRawArguementValue);
  printf("  Actual:\n");
  printf("    foo.exe %s\n", oEscapedArgument.c_str());
  printf("  Validating with system's cmd.exe...\n");
  printf("    argv[1]=%s\n", oSystemArgumentValue.c_str());
}

TEST_F(ArgumentManager, testGetCommandLineArgument)
{
  printf("\n");

  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "malicious argument\"&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestGetCommandLineArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\\"";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestGetCommandLineArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "\\\"hello\\ world";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestGetCommandLineArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestGetCommandLineArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------
  {
    const char * argumentValue      = "test\\\"&whoami";
    std::string    actualEscapedArg;
    std::string systemArgumentValue;

    prepareTestGetCommandLineArgument(argumentValue, actualEscapedArg, systemArgumentValue);

    //assert
    ASSERT_CSTR_EQ(systemArgumentValue.c_str(), argumentValue);
    printf("good!\n");
  }
  //-----------------------------------------------------------------------------------


}

TEST_F(ArgumentManager, testInsertEnd)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  std::string newArg = "new=true";
  ASSERT_TRUE( m.insert(newArg.c_str()) );

  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc+1, argc2);
  ASSERT_CSTR_EQ(argv[0], m.getArgument(0));
  ASSERT_CSTR_EQ(argv[1], m.getArgument(1));
  ASSERT_CSTR_EQ(argv[2], m.getArgument(2));
  ASSERT_CSTR_EQ(argv[3], m.getArgument(3));
  ASSERT_CSTR_EQ(newArg.c_str(), m.getArgument(4));
  ASSERT_CSTR_EQ("", m.getArgument(5));
  ASSERT_CSTR_EQ("", m.getArgument(6));
  ASSERT_CSTR_EQ("", m.getArgument(7));
}

TEST_F(ArgumentManager, testInsertEndAbsolutePos)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  std::string newArg = "new=true";
  ASSERT_TRUE( m.insert(4, newArg.c_str()) );

  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc+1, argc2);
  ASSERT_CSTR_EQ(argv[0], m.getArgument(0));
  ASSERT_CSTR_EQ(argv[1], m.getArgument(1));
  ASSERT_CSTR_EQ(argv[2], m.getArgument(2));
  ASSERT_CSTR_EQ(argv[3], m.getArgument(3));
  ASSERT_CSTR_EQ(newArg.c_str(), m.getArgument(4));
  ASSERT_CSTR_EQ("", m.getArgument(5));
  ASSERT_CSTR_EQ("", m.getArgument(6));
  ASSERT_CSTR_EQ("", m.getArgument(7));
}

TEST_F(ArgumentManager, testInsertOutOfBound)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  std::string newArg = "new=true";
  ASSERT_FALSE( m.insert(5, newArg.c_str()) );

  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  //no change in content
  ASSERT_EQ(argc, argc2);
  ASSERT_CSTR_EQ(argv[0], argv2[0]);
  ASSERT_CSTR_EQ(argv[1], argv2[1]);
  ASSERT_CSTR_EQ(argv[2], argv2[2]);
  ASSERT_CSTR_EQ(argv[3], argv2[3]);
  ASSERT_EQ(argv[4], argv2[4]);
}

TEST_F(ArgumentManager, testInsertFirst)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  std::string newArg = "new=true";
  ASSERT_TRUE( m.insert(0, newArg.c_str()) );

  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc+1, argc2);
  ASSERT_CSTR_EQ(newArg.c_str(), m.getArgument(0));
  ASSERT_CSTR_EQ(argv[0], m.getArgument(1));
  ASSERT_CSTR_EQ(argv[1], m.getArgument(2));
  ASSERT_CSTR_EQ(argv[2], m.getArgument(3));
  ASSERT_CSTR_EQ(argv[3], m.getArgument(4));
  ASSERT_CSTR_EQ("", m.getArgument(5));
  ASSERT_CSTR_EQ("", m.getArgument(6));
  ASSERT_CSTR_EQ("", m.getArgument(7));
}

TEST_F(ArgumentManager, testInsertMiddle)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  std::string newArg = "new=true";
  ASSERT_TRUE( m.insert(2, newArg.c_str()) );

  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc+1, argc2);
  ASSERT_CSTR_EQ(argv[0], m.getArgument(0));
  ASSERT_CSTR_EQ(argv[1], m.getArgument(1));
  ASSERT_CSTR_EQ(newArg.c_str(), m.getArgument(2));
  ASSERT_CSTR_EQ(argv[2], m.getArgument(3));
  ASSERT_CSTR_EQ(argv[3], m.getArgument(4));
  ASSERT_CSTR_EQ("", m.getArgument(5));
  ASSERT_CSTR_EQ("", m.getArgument(6));
  ASSERT_CSTR_EQ("", m.getArgument(7));
}

TEST_F(ArgumentManager, testRemoveEnd)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {"test.exe", "/p", "-logfile=log.txt", NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.remove(3) );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(ArgumentManager, testRemoveFirst)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {"/p", "-logfile=log.txt", "count=5", NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.remove(0) );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(ArgumentManager, testRemoveMiddle)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {"test.exe", "/p", "count=5", NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.remove(2) );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(ArgumentManager, testRemoveOutOfBounds)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act & assert
  ASSERT_FALSE( m.remove(4) );
}

TEST_F(ArgumentManager, testRemoveAll)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.remove(0) );
  ASSERT_TRUE( m.remove(0) );
  ASSERT_TRUE( m.remove(0) );
  ASSERT_TRUE( m.remove(0) );
  ASSERT_FALSE( m.remove(0) ); //already empty

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(ArgumentManager, testReplaceEnd)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {"test.exe", "/p", "-logfile=log.txt", "count=1", NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.replace(3, "count=1") );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(ArgumentManager, testReplaceFirst)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {"good.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.replace(0, "good.exe") );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(ArgumentManager, testReplaceMiddle)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {"test.exe", "/p", "/q", "count=5", NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.replace(2, "/q") );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(ArgumentManager, testReplaceOutOfBounds)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //act & assert
  ASSERT_FALSE( m.replace(4, "") );
}

TEST_F(ArgumentManager, testFindIndex)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //assert
  ASSERT_EQ( m.findIndex(""), -1 );
  ASSERT_EQ( m.findIndex("test.exe"), 0 );
  ASSERT_EQ( m.findIndex("/p"), 1 );
  ASSERT_EQ( m.findIndex("-logfile=log.txt"), 2 );
  ASSERT_EQ( m.findIndex("count=5"), 3 );
  ASSERT_EQ( m.findIndex(NULL), -1 );
}

TEST_F(ArgumentManager, testFindOption)
{
  //prepare
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //assert
  //ASSERT_OPTION( "", true, -1 );
  int index = 0;
#define ASSERT_FINDOPTION( text, result, expectedIndex) ASSERT_TRUE( m.findOption(text, index) == result ); ASSERT_EQ( index, expectedIndex );
  ASSERT_FINDOPTION( "", false, -1 );
  ASSERT_FINDOPTION( "test.exe", true, 0 );
  ASSERT_FINDOPTION( "/p", true, 1 );
  ASSERT_FINDOPTION( "-logfile=log.txt", true, 2 );
  ASSERT_FINDOPTION( "count=5", true, 3 );
  ASSERT_FINDOPTION( NULL, false, -1 );
#undef ASSERT_OPTION
}

TEST_F(ArgumentManager, testExtractOption)
{
  //prepare
  char* argv[] = {"test.exe", "/p=7", "/p", "-logfile=log.txt", "--help", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //assert
  ASSERT_FALSE( m.extractOption("/noexist") );
  ASSERT_TRUE ( m.getArgc() == argc );
  ASSERT_TRUE ( m.extractOption("/p") );
  ASSERT_TRUE ( m.getArgc() == argc - 1 );
  ASSERT_FALSE( m.extractOption("/p") );
  ASSERT_TRUE ( m.getArgc() == argc - 1 );
  ASSERT_FALSE( m.extractOption("/c") );
  ASSERT_TRUE ( m.getArgc() == argc-1 );
  ASSERT_TRUE ( m.extractOption("--help") );
  ASSERT_TRUE ( m.getArgc() == argc - 2 );
  ASSERT_FALSE( m.extractOption("--help") );
  ASSERT_TRUE ( m.getArgc() == argc - 2 );
}

TEST_F(ArgumentManager, testExtractValue)
{
  //prepare
  char* argv[] = {"test.exe", "/p=7", "/p", "-logfile=log.txt", "--help", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);

  //assert
  std::string value;
#define ASSERT_EXTRACT( text, result, expectedValue) ASSERT_TRUE( m.extractValue(text, value) == result ); ASSERT_EQ( value, expectedValue );
  ASSERT_EXTRACT( "/noexist", false, "" );
  ASSERT_TRUE   ( m.getArgc() == argc );
  ASSERT_EXTRACT( "-logfile=", true, "log.txt" );
  ASSERT_TRUE   ( m.getArgc() == argc - 1 );
  ASSERT_EXTRACT( "/p=", true, "7" );
  ASSERT_TRUE   ( m.getArgc() == argc - 2 );
  ASSERT_EXTRACT( "count=", true, "5" );
  ASSERT_TRUE   ( m.getArgc() == argc - 3 );
  ASSERT_EXTRACT( "-name=", false, "" );
  ASSERT_TRUE   ( m.getArgc() == argc - 3 );
#undef ASSERT_EXTRACT
}

TEST_F(ArgumentManager, testCopyCtor)
{
  ArgumentManager a;

  a.init("a b c d e");

  ArgumentManager b(a);

  ASSERT_TRUE( a.getArgc() == b.getArgc() );
  ASSERT_TRUE( a == b );
}

TEST_F(ArgumentManager, testOperatorEqual) //operator ==
{
  ArgumentManager a;
  ArgumentManager b;

  a.init("a b c d e");
  b.init("a b c d e");

  //identical
  ASSERT_TRUE ( a == b );
  ASSERT_FALSE( a != b );

  a.remove(4);
  ASSERT_FALSE( a == b );
  ASSERT_TRUE ( a != b );
  b.remove(4);

  //identical
  ASSERT_TRUE ( a == b );
  ASSERT_FALSE( a != b );

  a.remove(0);
  ASSERT_FALSE( a == b );
  ASSERT_TRUE ( a != b );
  b.remove(0);

  //identical
  ASSERT_TRUE ( a == b );
  ASSERT_FALSE( a != b );

  a.replace(0, "B");
  ASSERT_FALSE( a == b );
  ASSERT_TRUE ( a != b );
  b.replace(0, "B");

  //identical
  ASSERT_TRUE ( a == b );
  ASSERT_FALSE( a != b );
}

TEST_F(ArgumentManager, testAssignmentOperator) //operator =
{
  ArgumentManager a;

  a.init("a b c d e");

  ArgumentManager b;
  b = a;

  ASSERT_TRUE( a.getArgc() == b.getArgc() );
  ASSERT_TRUE( a == b );
}
