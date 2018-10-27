#include "TestArgumentList.h"
#include "TestUtils.h"
#include "libargvcodec/ArgumentList.h"
#include "ArgumentLister.h"
#include "rapidassist/strings.h"

#include <string.h> //for strdup()

using namespace libargvcodec;

#define ASSERT_CSTR_EQ(val1, val2) ASSERT_EQ(std::string(val1), std::string(val2))

void setDefaultOptionPrefix(ArgumentList & m)
{
  m.clearOptionPrefixes();
  m.addOptionPrefix(""); //no prefix
  m.addOptionPrefix("-");
  m.addOptionPrefix("/");
  m.addOptionPrefix("--");
}

bool isIdentical(ArgumentList & m, int expectedArgc, char ** expectedArgv)
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

void TestArgumentList::SetUp()
{
}

void TestArgumentList::TearDown()
{
  clearDynamicStrings();
}

TEST_F(TestArgumentList, testInitArgcArgv)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
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

TEST_F(TestArgumentList, testGetArgument)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
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

TEST_F(TestArgumentList, testInitVector)
{
  std::vector<std::string> args;
  args.push_back("test.exe");
  args.push_back("/p");
  args.push_back("-logfile=log.txt");
  args.push_back("count=5");
  int argc = (int)args.size();

  ArgumentList m;
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

TEST_F(TestArgumentList, testInsertEndAbsolutePos)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
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

TEST_F(TestArgumentList, testInsertOutOfBound)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
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

TEST_F(TestArgumentList, testInsertFirst)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
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

TEST_F(TestArgumentList, testInsertMiddle)
{
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
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

TEST_F(TestArgumentList, testRemoveEnd)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.remove(3) );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(TestArgumentList, testRemoveFirst)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.remove(0) );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(TestArgumentList, testRemoveMiddle)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("count=5"), NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.remove(2) );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(TestArgumentList, testRemoveOutOfBounds)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act & assert
  ASSERT_FALSE( m.remove(4) );
}

TEST_F(TestArgumentList, testRemoveAll)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentList m;
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

TEST_F(TestArgumentList, testReplaceEnd)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=1"), NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.replace(3, "count=1") );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(TestArgumentList, testReplaceFirst)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {mkstr("good.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.replace(0, "good.exe") );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(TestArgumentList, testReplaceMiddle)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  char* expectedArgv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("/q"), mkstr("count=5"), NULL};
  int expectedArgc = sizeof(expectedArgv)/sizeof(expectedArgv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act
  ASSERT_TRUE( m.replace(2, "/q") );

  //assert
  ASSERT_TRUE( isIdentical(m, expectedArgc, expectedArgv) == true );
}

TEST_F(TestArgumentList, testReplaceOutOfBounds)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //act & assert
  ASSERT_FALSE( m.replace(4, "") );
}

TEST_F(TestArgumentList, testFindIndex)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //assert
  ASSERT_EQ( m.findIndex(""), -1 );
  ASSERT_EQ( m.findIndex("test.exe"), 0 );
  ASSERT_EQ( m.findIndex("/p"), 1 );
  ASSERT_EQ( m.findIndex("-logfile=log.txt"), 2 );
  ASSERT_EQ( m.findIndex("count=5"), 3 );
  ASSERT_EQ( m.findIndex(NULL), -1 );
  bool caseSensitive = false;
  ASSERT_EQ( m.findIndex("", caseSensitive), -1 );
  ASSERT_EQ( m.findIndex("test.ExE", caseSensitive), 0 );
  ASSERT_EQ( m.findIndex("/P", caseSensitive), 1 );
  ASSERT_EQ( m.findIndex("-logFILE=log.txt", caseSensitive), 2 );
  ASSERT_EQ( m.findIndex("Count=5", caseSensitive), 3 );
}

TEST_F(TestArgumentList, testFindOption)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //assert
  int index = 0;
#define ASSERT_FINDOPTION( text, result, expectedIndex) ASSERT_TRUE( m.findOption(text, index) == result ); ASSERT_EQ( index, expectedIndex );
  ASSERT_FINDOPTION( "", false, -1 );
  ASSERT_FINDOPTION( "test.exe", true, 0 );
  ASSERT_FINDOPTION( "/p", true, 1 );
  ASSERT_FINDOPTION( "-logfile=log.txt", true, 2 );
  ASSERT_FINDOPTION( "count=5", true, 3 );
  ASSERT_FINDOPTION( NULL, false, -1 );
#undef ASSERT_FINDOPTION
  bool caseSensitive = false;
#define ASSERT_FINDOPTION( text, result, expectedIndex) ASSERT_TRUE( m.findOption(text, caseSensitive, index) == result ); ASSERT_EQ( index, expectedIndex );
  ASSERT_FINDOPTION( "", false, -1 );
  ASSERT_FINDOPTION( "test.ExE", true, 0 );
  ASSERT_FINDOPTION( "/P", true, 1 );
  ASSERT_FINDOPTION( "-logFILE=log.txt", true, 2 );
  ASSERT_FINDOPTION( "Count=5", true, 3 );
  ASSERT_FINDOPTION( NULL, false, -1 );
#undef ASSERT_FINDOPTION
}

TEST_F(TestArgumentList, testExtractOption)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p=7"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("--help"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
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

  //arrange (again)
  m.init(argc, argv);

  //assert (again)
  bool caseSensitive = false;
  ASSERT_FALSE( m.extractOption("/noEXIST", caseSensitive) );
  ASSERT_TRUE ( m.getArgc() == argc );
  ASSERT_TRUE ( m.extractOption("/P", caseSensitive) );
  ASSERT_TRUE ( m.getArgc() == argc - 1 );
  ASSERT_FALSE( m.extractOption("/c", caseSensitive) );
  ASSERT_TRUE ( m.getArgc() == argc - 1 );
  ASSERT_TRUE ( m.extractOption("--hELp", caseSensitive) );
  ASSERT_TRUE ( m.getArgc() == argc - 2 );
}

TEST_F(TestArgumentList, testExtractValue)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/p=7"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("--help"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //assert (default case sensitive)
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

  //arrange (again)
  m.init(argc, argv);

  //assert (again, not case sensitive)
  bool caseSensitive = false;
#define ASSERT_EXTRACT( text, result, expectedValue) ASSERT_TRUE( m.extractValue(text, caseSensitive, value) == result ); ASSERT_EQ( value, expectedValue );
  ASSERT_EXTRACT( "/noEXIST", false, "" );
  ASSERT_TRUE   ( m.getArgc() == argc );
  ASSERT_EXTRACT( "-logFILE=", true, "log.txt" );
  ASSERT_TRUE   ( m.getArgc() == argc - 1 );
  ASSERT_EXTRACT( "/P=", true, "7" );
  ASSERT_TRUE   ( m.getArgc() == argc - 2 );
  ASSERT_EXTRACT( "COunt=", true, "5" );
  ASSERT_TRUE   ( m.getArgc() == argc - 3 );
  ASSERT_EXTRACT( "-Name=", false, "" );
  ASSERT_TRUE   ( m.getArgc() == argc - 3 );
#undef ASSERT_EXTRACT

  //arrange (again)
  m.init(argc, argv);
  setDefaultOptionPrefix(m);

  //assert (again, not case sensitive, with wrong prefixes)
#define ASSERT_EXTRACT( text, result, expectedValue) ASSERT_TRUE( m.extractValue(text, false /*not case sensitive*/, value) == result ); ASSERT_EQ( value, expectedValue );
  ASSERT_EXTRACT( "/noEXIST", false, "" );
  ASSERT_TRUE   ( m.getArgc() == argc );
  ASSERT_EXTRACT( "logFILE=", true, "log.txt" ); //use "" instead of -
  ASSERT_TRUE   ( m.getArgc() == argc - 1 );
  ASSERT_EXTRACT( "-P=", true, "7" ); //use - instead of /
  ASSERT_TRUE   ( m.getArgc() == argc - 2 );
  ASSERT_EXTRACT( "-COunt=", true, "5" ); //use - instead of ""
  ASSERT_TRUE   ( m.getArgc() == argc - 3 );
  ASSERT_EXTRACT( "-Name=", false, "" );
  ASSERT_TRUE   ( m.getArgc() == argc - 3 );
#undef ASSERT_EXTRACT
}

TEST_F(TestArgumentList, testExtractNextValue)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/name"), mkstr("foo"), mkstr("/repeat"), mkstr("5"), mkstr("/inputfile"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //assert
  std::string value;
#define ASSERT_EXTRACTNEXT( text, result, expectedValue, expectedListSize) ASSERT_TRUE( m.extractNextValue(text, value) == result ); ASSERT_EQ( value, expectedValue ); ASSERT_EQ( m.getArgc(), expectedListSize );
  ASSERT_EXTRACTNEXT( "/repeat",     true,   "5", argc-2 );
  ASSERT_EXTRACTNEXT( "/inputfile", false,   "",  argc-2 );
  ASSERT_EXTRACTNEXT( "/name",       true, "foo", argc-4 );
#undef ASSERT_EXTRACTNEXT

  //arrange (again)
  m.init(argc, argv);

  //assert (again)
  bool caseSensitive = false;
#define ASSERT_EXTRACTNEXT( text, result, expectedValue, expectedListSize) ASSERT_TRUE( m.extractNextValue(text, caseSensitive, value) == result ); ASSERT_EQ( value, expectedValue ); ASSERT_EQ( m.getArgc(), expectedListSize );
  ASSERT_EXTRACTNEXT( "/rePEAT",     true,   "5", argc-2 );
  ASSERT_EXTRACTNEXT( "/inputFILE", false,   "",  argc-2 );
  ASSERT_EXTRACTNEXT( "/nAMe",       true, "foo", argc-4 );
#undef ASSERT_EXTRACTNEXT
}

TEST_F(TestArgumentList, testCopyCtor)
{
  ArgumentList a;

  char * argv[] = {mkstr("a"), mkstr("b"), mkstr("c"), mkstr("d"), mkstr("e")};
  const int argc = sizeof(argv)/sizeof(argv[0]);

  a.init(argc, argv);

  ArgumentList b(a);

  ASSERT_TRUE( a.getArgc() == b.getArgc() );
  ASSERT_TRUE( a == b );
}

TEST_F(TestArgumentList, testOperatorEqual) //operator ==
{
  ArgumentList a;
  ArgumentList b;

  char * argv[] = {mkstr("a"), mkstr("b"), mkstr("c"), mkstr("d"), mkstr("e")};
  const int argc = sizeof(argv)/sizeof(argv[0]);

  a.init(argc, argv);
  b.init(argc, argv);

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

TEST_F(TestArgumentList, testAssignmentOperator) //operator =
{
  ArgumentList a;

  char * argv[] = {mkstr("a"), mkstr("b"), mkstr("c"), mkstr("d"), mkstr("e")};
  const int argc = sizeof(argv)/sizeof(argv[0]);

  a.init(argc, argv);

  ArgumentList b;
  b = a;

  ASSERT_TRUE( a.getArgc() == b.getArgc() );
  ASSERT_TRUE( a == b );
}

TEST_F(TestArgumentList, testFindNextValue)
{
  //arrange
  char* argv[] = {mkstr("test.exe"), mkstr("/name"), mkstr("foo"), mkstr("/repeat"), mkstr("5"), mkstr("/inputfile"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);

  //assert
  int index = 0;
  std::string stringValue;
  int intValue = 0;
#define ASSERT_FINDNEXTVALUE( name, value, callResult, expectedIndex, expectedValue) ASSERT_TRUE( m.findNextValue(name, index, value) == callResult ); ASSERT_EQ( index, expectedIndex ); ASSERT_EQ( value, expectedValue );
  ASSERT_FINDNEXTVALUE("/name",      stringValue, true,   1, "foo");
  ASSERT_FINDNEXTVALUE("/repeat",    stringValue, true,   3, "5");
  ASSERT_FINDNEXTVALUE("/repeat",    intValue,    true,   3, 5);
  ASSERT_FINDNEXTVALUE("/inputfile", stringValue, false, -1, "");
#undef ASSERT_FINDNEXTVALUE

  //arrange (again)
  m.init(argc, argv);

  //assert (again)
  bool caseSensitive = false;
#define ASSERT_FINDNEXTVALUE( name, value, callResult, expectedIndex, expectedValue) ASSERT_TRUE( m.findNextValue(name, caseSensitive, index, value) == callResult ); ASSERT_EQ( index, expectedIndex ); ASSERT_EQ( value, expectedValue );
  ASSERT_FINDNEXTVALUE("/nAMe",      stringValue, true,   1, "foo");
  ASSERT_FINDNEXTVALUE("/rePEAT",    stringValue, true,   3, "5");
  ASSERT_FINDNEXTVALUE("/rePEAT",    intValue,    true,   3, 5);
  ASSERT_FINDNEXTVALUE("/inputFILE", stringValue, false, -1, "");
#undef ASSERT_FINDNEXTVALUE

}

TEST_F(TestArgumentList, testWrongPrefixes)
{
  //prepare
  char* argv[] = {mkstr("test.exe"), mkstr("/p=7"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("--help"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);
  setDefaultOptionPrefix(m);

  int index = 0;
  std::string value;
#define ASSERT_FINDVALUE( text, expectedResult, expectedIndex, expectedValue) index = -1; value="foobar"; ASSERT_TRUE( m.findValue(text, false /*not case sensitive*/, index, value) == expectedResult ); ASSERT_EQ( index, expectedIndex ); ASSERT_EQ( value, expectedValue );
  ASSERT_FINDVALUE(   "/noEXIST",  false, -1,        "" );
  ASSERT_FINDVALUE(  "-logFILE=",   true,  3, "log.txt" ); //use actual prefix
  ASSERT_FINDVALUE(   "logFILE=",   true,  3, "log.txt" ); //use "" instead of -
  ASSERT_FINDVALUE(  "/logFILE=",   true,  3, "log.txt" ); //use / instead of -
  ASSERT_FINDVALUE( "--logFILE=",   true,  3, "log.txt" ); //use -- instead of -
#undef ASSERT_FINDVALUE

#define ASSERT_FINDOPTION( text, expectedResult, expectedIndex) index = -1; ASSERT_TRUE( m.findOption(text, false /*not case sensitive*/, index) == expectedResult ); ASSERT_EQ( index, expectedIndex );
  ASSERT_FINDOPTION( "/noEXIST",  false, -1);
  ASSERT_FINDOPTION(  "--help",    true,  4); //use actual prefix
  ASSERT_FINDOPTION(    "help",    true,  4); //use "" instead of --
  ASSERT_FINDOPTION(   "/help",    true,  4); //use / instead of --
  ASSERT_FINDOPTION(   "-help",    true,  4); //use - instead of --
#undef ASSERT_FINDOPTION

#define ASSERT_FINDNEXTVALUE( text, expectedResult, expectedIndex, expectedValue) index = -1; value="foobar"; ASSERT_TRUE( m.findNextValue(text, false /*not case sensitive*/, index, value) == expectedResult ); ASSERT_EQ( index, expectedIndex ); ASSERT_EQ( value, expectedValue );
  ASSERT_FINDNEXTVALUE("/noEXIST",  false, -1,        "" );
  ASSERT_FINDNEXTVALUE(  "--help",   true,  4, "count=5" ); //use actual prefix
  ASSERT_FINDNEXTVALUE(    "help",   true,  4, "count=5" ); //use "" instead of --
  ASSERT_FINDNEXTVALUE(   "/help",   true,  4, "count=5" ); //use / instead of --
  ASSERT_FINDNEXTVALUE(   "-help",   true,  4, "count=5" ); //use - instead of --
#undef ASSERT_FINDNEXTVALUE
}

TEST_F(TestArgumentList, testEqualize)
{
  //prepare
  char* argv[] = {mkstr("test.exe"), mkstr("/p=7"), mkstr("/p"), mkstr("-logfile=log.txt"), mkstr("--help"), mkstr("count=5"), NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentList m;
  m.init(argc, argv);
  setDefaultOptionPrefix(m);

  int index = 0;
  std::string value;
#define ASSERT_FINDVALUE( text, expectedResult, expectedIndex, expectedValue) index = -1; value="foobar"; ASSERT_TRUE( m.findValue(text, false /*not case sensitive*/, index, value) == expectedResult ); ASSERT_EQ( index, expectedIndex ); ASSERT_EQ( value, expectedValue );
  ASSERT_FINDVALUE(   "/noEXIST",  false, -1,        "" );
  ASSERT_FINDVALUE(  "-logFILE=",   true,  3, "log.txt" );
  ASSERT_FINDVALUE(   "-logFILE",   true,  3, "log.txt" );
  ASSERT_FINDVALUE(   "-LOGFILE",   true,  3, "log.txt" );
  ASSERT_FINDVALUE(   "-logfile",   true,  3, "log.txt" );
#undef ASSERT_FINDVALUE
}