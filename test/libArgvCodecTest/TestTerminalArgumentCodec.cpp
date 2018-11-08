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
    //Rule 2.
    const std::string value = "a b c";
    const std::string expected = "\"a b c\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string value = "Toys \"R\" Us";
    const std::string expected = "'Toys \"R\" Us'";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 2.
    const std::string value = "McDonald's I'm Lovin' It";
    const std::string expected = "\"McDonald's I'm Lovin' It\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 4.
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
    //Rule 5.3.
    const std::string value = "a * b";
    const std::string expected = "\"a \\* b\"";

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
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.4.
    const std::string value = "tRuE";
    const std::string expected = "\"tRuE\"";

    libargvcodec::TerminalArgumentCodec codec;
    std::string actual = codec.encodeArgument(value.c_str());

    //assert
    ASSERT_EQ(expected, actual);
  }
  //-----------------------------------------------------------------------------------
  {
    //Rule 5.4.
    const std::string value = "fAlSe";
    const std::string expected = "\"fAlSe\"";

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
    const std::string cmdline = "\"a b c\"'d e'";

    libargvcodec::TerminalArgumentCodec codec;
    ArgumentList args = codec.decodeCommandLine(cmdline.c_str());

    //assert
    ASSERT_EQ(2, args.getArgc());
    const char * arg1 = args.getArgument(1);
    const char * arg2 = args.getArgument(2);
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
    const std::string cmdline = "'a \\'b' c"; //   'a \'b' c

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
    const std::string cmdline = "'McDonald\\'s I\\'m Lovin\\' It'"; //     'McDonald\'s I\'m Lovin\' It'

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

std::string getShowArgsExecutablePath()
{
  std::string path = ra::process::getCurrentProcessPath();
  int numTokenReplaced = ra::strings::replace(path, "libargvcodec_unittest", "showargs");
  return path;
}

TEST_F(TestTerminalArgumentCodec, testSystem)
{
  std::string test_file = ra::gtesthelp::getTestQualifiedName() + ".txt";
  ASSERT_TRUE( ra::filesystem::fileExists(test_file.c_str()) );

  //load file in memory
  ra::strings::StringVector content;
  bool readSuccess = ra::gtesthelp::getTextFileContent(test_file.c_str(), content);
  ASSERT_TRUE( readSuccess );

  //define a tmp file for testing
  std::string tmp_file = ra::gtesthelp::getTestQualifiedName() + ".tmp";

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
        //build command line
        std::string cmdline;
        cmdline = getShowArgsExecutablePath();
        cmdline += " ";
        cmdline += commands[0];

        //configure the showargs call to output to a file
        cmdline += " >" + tmp_file;

        //remove the command line from commands
        commands.erase(commands.begin());

        //remaining commands are expected arguments
        ra::strings::StringVector expected_arguments = commands;

        //delete the tmp file if it already exists
        if (ra::filesystem::fileExists(tmp_file.c_str()))
        {
          bool deleted = ra::filesystem::deleteFile(tmp_file.c_str());
          ASSERT_TRUE( deleted );
        }

        //execute the command line
        int return_code = system(cmdline.c_str());
        ASSERT_EQ(0, return_code) << "cmdline=" << cmdline.c_str();

        //expect the tmp file was created
        ASSERT_TRUE(ra::filesystem::fileExists(tmp_file.c_str()));

        //parse 'showargs' output
        ra::strings::StringVector actual_arguments;
        readSuccess = ra::gtesthelp::getTextFileContent(tmp_file.c_str(), actual_arguments);
        ASSERT_TRUE( readSuccess );

        //cleanup "argX: " line prefix.
        for(size_t j=0; j<actual_arguments.size(); j++)
        {
          std::string prefix = "arg" + ra::strings::toString(j) + ": ";
          std::string & actual_argument = actual_arguments[j];
          int numTokenReplaced = ra::strings::replace(actual_argument, prefix.c_str(), "");
        }

        //remove arg[0] from the list
        if (actual_arguments.size() > 0)
        {
          actual_arguments.erase(actual_arguments.begin());
        }

        //delete the tmp file if it already exists
        if (ra::filesystem::fileExists(tmp_file.c_str()))
        {
          bool deleted = ra::filesystem::deleteFile(tmp_file.c_str());
          ASSERT_TRUE( deleted ) << "Unable to delete file \"" << tmp_file.c_str() << ".";
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
        for(size_t j=0; j<actual_arguments.size(); j++)
        {
          error_message += "  arg[" + ra::strings::toString(j) + "]: ";
          error_message += actual_arguments[j] + "\n";
        }

        //assert actual arguments are equals to expected arguments
        ASSERT_EQ(expected_arguments.size(), actual_arguments.size()) << error_message;
        for(size_t j=0; j<expected_arguments.size(); j++)
        {
          const std::string & expected_argument = expected_arguments[j];
          const std::string & actual_argument   = actual_arguments[j];
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