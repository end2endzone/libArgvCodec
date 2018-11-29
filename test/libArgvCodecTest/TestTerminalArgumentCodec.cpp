#include "TestTerminalArgumentCodec.h"
#include "libargvcodec/TerminalArgumentCodec.h"
#include "rapidassist/strings.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/process.h"
#include "rapidassist/cppencoder.h"
#include "TestUtils.h"

using namespace libargvcodec;

void TestTerminalArgumentCodec::SetUp()
{
}

void TestTerminalArgumentCodec::TearDown()
{
}

TEST_F(TestTerminalArgumentCodec, testEncodeArgument)
{
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.2.
    const std::string value = "a b c";
    const std::string expected = "\"a b c\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rules 3.3.
    const std::string value = "Toys \"R\" Us";
    const std::string expected = "'Toys \"R\" Us'";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.2.
    const std::string value = "McDonald's I'm Lovin' It";
    const std::string expected = "\"McDonald's I'm Lovin' It\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 4.1.
    const std::string value = "a\\b";
    const std::string expected = "a\\\\b";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.1.
    const std::string value = "a < b";
    const std::string expected = "\"a < b\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.1.
    const std::string value = "a & b";
    const std::string expected = "\"a & b\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.2.
    const std::string value = "a<b";
    const std::string expected = "a\\<b";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.2.
    const std::string value = "a&b";
    const std::string expected = "a\\&b";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.1.
    const std::string value = "a * b";
    const std::string expected = "\"a * b\""; //    "a * b"

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.3.
    const std::string value = "sum=$42";
    const std::string expected = "sum=\\$42";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.3.
    const std::string value = "sum = $20";
    const std::string expected = "\"sum = \\$20\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
}

TEST_F(TestTerminalArgumentCodec, testDecodeCommandLine)
{
  //-----------------------------------------------------------------------------------
  {
    //Rule 6.1 & Rule 2.4.
    const std::string cmdline = "\"\"\"\"";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 6.1 & Rule 2.4.
    const std::string cmdline = "a\"\"b";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "ab");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 1.
    const std::string cmdline = "a b c";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(4, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    const char * arg3 = args.getArgument(3);
    ASSERT_STREQ(arg1, "a");
    ASSERT_STREQ(arg2, "b");
    ASSERT_STREQ(arg3, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string cmdline = "\"a b c\"";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "a b c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string cmdline = "a\" b\" c";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(3, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    ASSERT_STREQ(arg1, "a b");
    ASSERT_STREQ(arg2, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string cmdline = "\"a \"b c";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(3, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    ASSERT_STREQ(arg1, "a b");
    ASSERT_STREQ(arg2, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string cmdline = "'a b c'";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "a b c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string cmdline = "a' b' c";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(3, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    ASSERT_STREQ(arg1, "a b");
    ASSERT_STREQ(arg2, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string cmdline = "'a 'b c";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(3, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    ASSERT_STREQ(arg1, "a b");
    ASSERT_STREQ(arg2, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string cmdline = "\"a b c\"'d e'"; //  "a b c"'d e'

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "a b cd e");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "a \\\"b c"; //     a \"b c

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(4, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    const char * arg3 = args.getArgument(3);
    ASSERT_STREQ(arg1, "a");
    ASSERT_STREQ(arg2, "\"b");
    ASSERT_STREQ(arg3, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "\"a \\\"b\" c"; //   "a \"b" c

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(3, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    ASSERT_STREQ(arg1, "a \"b");
    ASSERT_STREQ(arg2, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "a \\'b c"; //       a \'b c

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(4, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    const char * arg3 = args.getArgument(3);
    ASSERT_STREQ(arg1, "a");
    ASSERT_STREQ(arg2, "'b");
    ASSERT_STREQ(arg3, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "'a '\\''b' c"; //   'a '\''b' c

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(3, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
    ASSERT_STREQ(arg1, "a 'b");
    ASSERT_STREQ(arg2, "c");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "\"McDonald's I'm Lovin' It\"";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "McDonald's I'm Lovin' It");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "'Toys \"R\" Us'"; //           'Toys "R" Us'

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "Toys \"R\" Us");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "\"Toys \\\"R\\\" Us\""; //     "Toys \"R\" Us"

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "Toys \"R\" Us");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 3.
    const std::string cmdline = "'McDonald'\\''s I'\\''m Lovin'\\'' It'"; //     'McDonald'\''s I'\''m Lovin'\'' It'

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "McDonald's I'm Lovin' It");
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 4.
    const std::string cmdline = "a\\\\b"; //     a\\b

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "a\\b"); //    a\b
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 4.
    const std::string cmdline = "\"a\\\\b\""; //     "a\\b"

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "a\\b"); //    a\b
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 4.
    const std::string cmdline = "a\\\\\"b c\""; //     a\\"b c"

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    ASSERT_STREQ(arg1, "a\\b c"); //    a\b c
  }
}

TEST_F(TestTerminalArgumentCodec, testDecodeCommandLine_testfile)
{
  //The objective of this unit test is to validate the content of file 'Test.CommandLines.Linux.txt' with TerminalArgumentCodec::decodeCommandLine() implementation.
  //On Windows systems, the content of file 'Test.CommandLines.Linux.txt' is also validated with a system() call.

  libargvcodec::TerminalArgumentCodec codec;

  std::string test_file = "Test.CommandLines.Linux.txt";

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

#ifdef __linux__
    //On Linux system, also try to validate content of test file againts a system() call.

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
#endif //__linux__
  }
}

TEST_F(TestTerminalArgumentCodec, testEncodeCommandLine_testfile)
{
  libargvcodec::TerminalArgumentCodec codec;

  std::string test_file = "Test.CommandLines.Linux.txt";

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

#ifdef __linux__
    //On Linux system, also try to validate the generated command line with a system() call.

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
#endif //__linux__

  }
}

TEST_F(TestTerminalArgumentCodec, testPrintableCharacters)
{
  libargvcodec::TerminalArgumentCodec codec;
 
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
 
#ifdef __linux__
    //On Linux system, also try to validate the generated command line with a system() call.
 
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
#endif // __linux__
  }
}
