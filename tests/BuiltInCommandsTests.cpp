#include "gtest/gtest.h"
#include "BuiltInCommands.h"
#include <string>

TEST(ChpromptTests, NewPrompt)
{
    ChangePromptCommand command = ChangePromptCommand("chprompt newPrompt");
    command.execute();
    EXPECT_EQ(SmallShell::getInstance().getPrompt(), "newPrompt> ");
}

TEST(ChpromptTests, NoArgs)
{
    ChangePromptCommand command = ChangePromptCommand("chprompt");
    command.execute();
    EXPECT_EQ(SmallShell::getInstance().getPrompt(), "smash> ");
}

TEST(ChpromptTests, TooManyArgs)
{
    ChangePromptCommand command = ChangePromptCommand("chprompt newPrompt test");
    command.execute();
    EXPECT_EQ(SmallShell::getInstance().getPrompt(), "newPrompt> ");
}

// TODO: empty string for chprompt?

TEST(ShowPidTests, BasicShowPid)
{
    ShowPidCommand command = ShowPidCommand("showpid");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();
    EXPECT_EQ(out.str(), "smash pid is " + std::to_string(getpid()) + "\n");
}

TEST(ShowPidTests, IgnoreArgs)
{
    ShowPidCommand command = ShowPidCommand("showpid arg1 arg2 arg3");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();
    EXPECT_EQ(out.str(), "smash pid is " + std::to_string(getpid()) + "\n");
}

TEST(PwdTests, BasicPwd)
{
    GetCurrDirCommand command = GetCurrDirCommand("pwd");
    std::ostringstream out;
    command.setOutputStream(&out);
    // Get the current working directory, and store it in a string.
    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string expected(buffer);

    command.execute();
    EXPECT_EQ(out.str(), expected + "\n");
}

TEST(PwdTests, IgnoreArgs)
{
    GetCurrDirCommand command = GetCurrDirCommand("pwd arg1 arg2 arg3");
    std::ostringstream out;
    command.setOutputStream(&out);
    // Get the current working directory, and store it in a string.
    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string expected(buffer);

    command.execute();
    EXPECT_EQ(out.str(), expected + "\n");
}

TEST(PwdTests, ChangeDirectory)
{
    ChangeDirCommand command = ChangeDirCommand("cd /home");
    command.execute();

    GetCurrDirCommand command2 = GetCurrDirCommand("pwd");
    std::ostringstream out;
    command2.setOutputStream(&out);
    command2.execute();

    EXPECT_EQ(out.str(), "/home\n");
}

TEST(CdTests, BasicChangeDirectory)
{
    ChangeDirCommand command = ChangeDirCommand("cd /home");
    command.execute();

    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string actual(buffer);

    EXPECT_EQ(actual, "/home");
}

TEST(CdTests, ChangePreviousDirectory)
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

TEST(CdTests, PrevFailed)
{
    SmallShell::getInstance().setPrevPath("");

    ChangeDirCommand command = ChangeDirCommand("cd -");

    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash error: cd: OLDPWD not set\n");
}

TEST(CdTests, TooManyArgs)
{
    ChangeDirCommand command = ChangeDirCommand("cd / test");

    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash error: cd: too many arguments\n");
}

TEST(CdTests, NonExistentDirectory)
{
    ChangeDirCommand command = ChangeDirCommand("cd /test");

    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash error: cd: /test: No such file or directory\n");
}

TEST(CdTests, EmptyString)
{
    ChangeDirCommand command = ChangeDirCommand("cd");

    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();
    // TODO: is this the correct output?
    EXPECT_EQ(out.str(), "smash error: cd: too few arguments\n");
}

// Test jobs command
TEST(JobsTests, BasicList)
{
    ExternalCommand *command1 = new ExternalCommand("sleep 5 &");
    ExternalCommand *command2 = new ExternalCommand("sleep 5 &");
    ExternalCommand *command3 = new ExternalCommand("sleep 5 &");

    command1->execute();
    command2->execute();
    command3->execute();

    JobsCommand command = JobsCommand("jobs");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    // compare by line

    std::string actual = out.str();
    std::string expected;
    // TODO: print the actual command received or the name?
    expected += "[1]sleep : " + std::to_string(command1->getPid()) + " 0 secs\n";
    expected += "[2]sleep : " + std::to_string(command2->getPid()) + " 0 secs\n";
    expected += "[3]sleep : " + std::to_string(command3->getPid()) + " 0 secs\n";

    EXPECT_EQ(expected, actual);
}

TEST(JobsTests, IgnoreArgs)
{
    JobsCommand command = JobsCommand("jobs arg1 arg2 arg3");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();
    EXPECT_EQ(out.str(), "");
}

// TODO: adding stopped job
// TODO: test sorted by id
// TODO: test removing finished jobs

// Test fg command

TEST(FgTests, BasicForeground)
{
    ExternalCommand *command1 = new ExternalCommand("sleep 2 &");
    command1->execute();

    ForegroundCommand command = ForegroundCommand("fg");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    // compare by line

    std::string actual = out.str();
    std::string expected;

    expected += "sleep : " + std::to_string(command1->getPid()) + "\n";

    EXPECT_EQ(expected, actual);
    SmallShell::getInstance().getJobsList().killAllJobs();
}

TEST(FgTests, ForegroundId)
{
    ExternalCommand *command1 = new ExternalCommand("sleep 2 &");
    command1->execute();

    ForegroundCommand command = ForegroundCommand("fg 1");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    // compare by line

    std::string actual = out.str();
    std::string expected;

    expected += "sleep : " + std::to_string(command1->getPid()) + "\n";

    EXPECT_EQ(expected, actual);
}

TEST(FgTests, StoppedJob)
{
    ExternalCommand *command1 = new ExternalCommand("sleep 2 &");
    SmallShell::getInstance().getJobsList().addJob(command1, true);

    ForegroundCommand command = ForegroundCommand("fg 1");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    // compare by line

    std::string actual = out.str();
    std::string expected;

    expected += "sleep : " + std::to_string(command1->getPid()) + "\n";

    EXPECT_EQ(expected, actual);
}

TEST(FgTests, InvalidId)
{
    ForegroundCommand command = ForegroundCommand("fg 1");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash error: fg: job-id 1 does not exist\n");
}

TEST(FgTests, TooManyArgs)
{
    ForegroundCommand command = ForegroundCommand("fg 1 2 3");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash error: fg: invalid arguments\n");
}

// Test bg command

// TODO: test bg with stopped job

// Test quit command

TEST(QuitTests, BasicQuit)
{
    QuitCommand command = QuitCommand("quit");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "");
}

TEST(QuitTests, IgnoreArgs)
{
    QuitCommand command = QuitCommand("quit arg1 arg2 arg3");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();
    EXPECT_EQ(out.str(), "");
}

TEST(QuitTests, QuitKill)
{
    ExternalCommand *command1 = new ExternalCommand("sleep 2 &");
    command1->execute();

    QuitCommand command = QuitCommand("quit");
    std::ostringstream out;
    command.setOutputStream(&out);
    command.execute();

    EXPECT_EQ(out.str(), "smash: sending SIGKILL signal to 1 jobs:\n[1]sleep : " + std::to_string(command1->getPid()) + "\n");
}
