#include "gtest_emulator.h"
#include "ArgumentManager.h"

#define ASSERT_CSTR_EQ(val1, val2) ASSERT_EQ(std::string(val1), std::string(val2))

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
