#include "TestTerminalArgumentCodec.h"
#include "libargvcodec/TerminalArgumentCodec.h"
#include "ArgumentLister.h"
#include "rapidassist/strings.h"
#include "TestUtils.h"
#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"

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