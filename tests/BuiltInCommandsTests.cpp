#include "gtest/gtest.h"
#include "BuiltInCommands.h"
#include <string>

TEST(BuiltInCommandsTests, TestBuiltInCommands)
{
    EXPECT_EQ(0, 0);
}

TEST(GetCurrentDirectoryTests, TestGetCurrentDirectory)
{
    GetCurrDirCommand command = GetCurrDirCommand("pwd");
    std::ostringstream out;
    command.setOutputStream(&out);
    // Get the current working directory, and store it in a string.
    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string actual(buffer);

    command.execute();
    EXPECT_EQ(out.str(), actual + "\n");
}

TEST(ChangeCurrentDirectoryTests, BasicChangeDirectory)
{
    ChangeDirCommand command = ChangeDirCommand("cd /home");
    command.execute();

    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string actual(buffer);

    EXPECT_EQ(actual, "/home");
}

TEST(ChangeCurrentDirectoryTests, ChangePreviousDirectory)
{
    std::string path = "cd " + SmallShell::getInstance().getWorkingDir();
    ChangeDirCommand command = ChangeDirCommand(path.c_str());
    command.execute();

    ChangeDirCommand command2 = ChangeDirCommand("cd /");
    command2.execute();

    ChangeDirCommand command3 = ChangeDirCommand("cd -");
    command3.execute();

    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string actual(buffer);

    EXPECT_EQ(actual, SmallShell::getInstance().getWorkingDir());
}

TEST(ChangeCurrentDirectoryTests, PrevFailed)
{
    SmallShell::getInstance().setPrevPath("");

    ChangeDirCommand command = ChangeDirCommand("cd -");

    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash error: cd: OLDPWD not set\n");
}

TEST(ChangeCurrentDirectoryTests, TooManyArgs)
{
    ChangeDirCommand command = ChangeDirCommand("cd / test");

    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash error: cd: too many arguments\n");
}