#include "TestTerminalArgumentCodec.h"
#include "libargvcodec/TerminalArgumentCodec.h"
#include "ArgumentLister.h"
#include "rapidassist/strings.h"
#include "TestUtils.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/process.h"

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

bool isDashedLine(const std::string & iLine)
{
  if (iLine.size() < 10) //must be at least 10 dash characters long
    return false;

  for(size_t i=0; i<iLine.size(); i++)
  {
    if (iLine[i] != '-')
      return false; //not a dash character
  }

  return true;
}

TEST_F(TestTerminalArgumentCodec, testSystem)
{
  //The objective of this unit test is to validate the content of 
  //file 'TestTerminalArgumentCodec.testSystem.txt' against the following:
  //  1) system() call on Linux system.
  //  2) TerminalArgumentCodec::decodeCommandLine() on all platforms

  std::string test_file = ra::gtesthelp::getTestQualifiedName() + ".txt";
  ASSERT_TRUE( ra::filesystem::fileExists(test_file.c_str()) );

  //load file in memory
  ra::strings::StringVector content;
  bool readSuccess = ra::gtesthelp::getTextFileContent(test_file.c_str(), content);
  ASSERT_TRUE( readSuccess );

  libargvcodec::TerminalArgumentCodec codec;

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

#ifdef __linux__
        //On Linux system, also try to validate content of test file againts a system() call.

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
#endif //__linux__
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

TEST_F(TestTerminalArgumentCodec, testLinuxCommandLine)
{
  std::string test_file = "TestTerminalArgumentCodec.testSystem.txt";
  ASSERT_TRUE( ra::filesystem::fileExists(test_file.c_str()) );

  //load file in memory
  ra::strings::StringVector content;
  bool readSuccess = ra::gtesthelp::getTextFileContent(test_file.c_str(), content);
  ASSERT_TRUE( readSuccess );

  libargvcodec::TerminalArgumentCodec codec;

  //process each command lines
  ra::strings::StringVector commands;
  for(size_t i=0; i<content.size(); i++)
  {
    const std::string & line = content[i];
    if (isDashedLine(line))
    {
      //process current commands
      if (commands.size() >= 2) //must be at least 2 lines long (a cmdline and at least 1 argument)
      {
        //get command line
        std::string cmdline = commands[0];

        //remove the command line from commands
        commands.erase(commands.begin());

        //remaining commands are expected arguments
        ra::strings::StringVector expected_arguments = commands;

        //decode with TerminalArgumentCodec
        ArgumentList actual_arguments = codec.decodeCommandLine(cmdline.c_str());

        //remove arg[0] from the list
        if (actual_arguments.getArgc() > 0)
        {
          actual_arguments.remove(0);
        }

        //build a meaningful error message
        std::string error_message;
        error_message += "The command line `" + cmdline + "`\n";
        error_message += "was expected to return the following arguments:\n";
        for(size_t j=0; j<expected_arguments.size(); j++)
        {
          error_message += "  arg[" + ra::strings::toString(j) + "]: ";
          error_message += expected_arguments[j] + "\n";
        }
        error_message += "but actually returned the following arguments:\n";
        for(int j=0; j<actual_arguments.getArgc(); j++)
        {
          error_message += "  arg[" + ra::strings::toString(j) + "]: ";
          error_message += actual_arguments.getArgument(j);
          error_message += "\n";
        }

        //assert actual arguments are equals to expected arguments
        ASSERT_EQ((int)expected_arguments.size(), actual_arguments.getArgc()) << error_message;
        for(size_t j=0; j<expected_arguments.size(); j++)
        {
          const std::string & expected_argument = expected_arguments[j];
          const std::string   actual_argument   = actual_arguments.getArgument((int)j);
          ASSERT_EQ(expected_argument, actual_argument) << error_message;
        }
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