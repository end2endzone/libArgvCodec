#include "gtest_emulator.h"
#include "ArgumentManager.h"

#define ASSERT_STR_EQ(val1, val2) ASSERT_EQ(std::string(val1), std::string(val2))

TEST_F(ArgumentManager, testInitArgcArgv)
{
  char* argv[] = {"test.exe", "/p", "-logfile=log.txt", "count=5", NULL};
  int argc = sizeof(argv)/sizeof(argv[0]) - 1;

  ArgumentManager m;
  m.init(argc, argv);
  int argc2 = m.getArgc();
  char** argv2 = m.getArgv();

  ASSERT_EQ(argc, argc2);
  ASSERT_EQ(std::string(argv[0]), std::string(argv2[0]));
  ASSERT_EQ(std::string(argv[1]), std::string(argv2[1]));
  ASSERT_EQ(std::string(argv[2]), std::string(argv2[2]));
  ASSERT_EQ(std::string(argv[3]), std::string(argv2[3]));
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
  ASSERT_EQ(std::string(argv[0]), std::string(m.getArgument(0)));
  ASSERT_EQ(std::string(argv[1]), std::string(m.getArgument(1)));
  ASSERT_EQ(std::string(argv[2]), std::string(m.getArgument(2)));
  ASSERT_EQ(std::string(argv[3]), std::string(m.getArgument(3)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(4)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(5)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(6)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(7)));
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
  ASSERT_EQ(std::string(argv[0]), std::string(m.getArgument(0)));
  ASSERT_EQ(std::string(argv[1]), std::string(m.getArgument(1)));
  ASSERT_EQ(std::string(argv[2]), std::string(m.getArgument(2)));
  ASSERT_EQ(std::string(argv[3]), std::string(m.getArgument(3)));
  ASSERT_EQ(newArg, std::string(m.getArgument(4)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(5)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(6)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(7)));
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
  ASSERT_EQ(newArg, std::string(m.getArgument(0)));
  ASSERT_EQ(std::string(argv[0]), std::string(m.getArgument(1)));
  ASSERT_EQ(std::string(argv[1]), std::string(m.getArgument(2)));
  ASSERT_EQ(std::string(argv[2]), std::string(m.getArgument(3)));
  ASSERT_EQ(std::string(argv[3]), std::string(m.getArgument(4)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(5)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(6)));
  ASSERT_EQ(std::string(""), std::string(m.getArgument(7)));
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
