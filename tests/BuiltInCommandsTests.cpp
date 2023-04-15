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

// Test jobs command
TEST(JobsListTests, TestJobsList)
{
    ExternalCommand *command1 = new ExternalCommand("sleep 1");
    ExternalCommand *command2 = new ExternalCommand("sleep 2");
    ExternalCommand *command3 = new ExternalCommand("sleep 3");
    SmallShell::getInstance().getJobsList().addJob(command1, false);
    SmallShell::getInstance().getJobsList().addJob(command2, false);
    SmallShell::getInstance().getJobsList().addJob(command3, false);

    JobsCommand command = JobsCommand("jobs");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    // compare by line

    std::string expected = out.str();
    std::string actual;
    actual += "[1]sleep : " + std::to_string(command1->getPid()) + " 0 secs\n";
    actual += "[2]sleep : " + std::to_string(command2->getPid()) + " 0 secs\n";
    actual += "[3]sleep : " + std::to_string(command3->getPid()) + " 0 secs\n";

    EXPECT_EQ(expected, actual);
}

// Test fg command

TEST(ForegroundCommandTests, TestForegroundCommand)
{
    ExternalCommand *command1 = new ExternalCommand("sleep 1");
    ExternalCommand *command2 = new ExternalCommand("sleep 2");
    ExternalCommand *command3 = new ExternalCommand("sleep 3");
    SmallShell::getInstance().getJobsList().addJob(command1, false);
    SmallShell::getInstance().getJobsList().addJob(command2, false);
    SmallShell::getInstance().getJobsList().addJob(command3, false);

    ForegroundCommand command = ForegroundCommand("fg 2");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    // compare by line

    std::string expected = out.str();
    std::string actual;

    actual += "sleep : " + std::to_string(command2->getPid()) + "\n";

    EXPECT_EQ(expected, actual);
}