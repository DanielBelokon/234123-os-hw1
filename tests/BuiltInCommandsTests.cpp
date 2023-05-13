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
    testing::internal::CaptureStdout();
    command.execute();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "smash pid is " + std::to_string(getpid()) + "\n");
}

TEST(ShowPidTests, IgnoreArgs)
{
    ShowPidCommand command = ShowPidCommand("showpid arg1 arg2 arg3");
    testing::internal::CaptureStdout();
    command.execute();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "smash pid is " + std::to_string(getpid()) + "\n");
}

TEST(PwdTests, BasicPwd)
{
    GetCurrDirCommand command = GetCurrDirCommand("pwd");
    testing::internal::CaptureStdout();
    // Get the current working directory, and store it in a string.
    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string expected(buffer);

    command.execute();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), expected + "\n");
}

TEST(PwdTests, IgnoreArgs)
{
    GetCurrDirCommand command = GetCurrDirCommand("pwd arg1 arg2 arg3");
    testing::internal::CaptureStdout();
    // Get the current working directory, and store it in a string.
    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string expected(buffer);

    command.execute();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), expected + "\n");
}

TEST(PwdTests, ChangeDirectory)
{
    ChangeDirCommand command = ChangeDirCommand("cd /home");
    command.execute();

    GetCurrDirCommand command2 = GetCurrDirCommand("pwd");
    testing::internal::CaptureStdout();
    command2.execute();

    EXPECT_EQ(testing::internal::GetCapturedStdout(), "/home\n");
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

    testing::internal::CaptureStderr();
    command.execute();

    EXPECT_EQ(testing::internal::GetCapturedStderr(), "smash error: cd: OLDPWD not set\n");
}

TEST(CdTests, TooManyArgs)
{
    ChangeDirCommand command = ChangeDirCommand("cd / test");

    testing::internal::CaptureStderr();
    command.execute();

    EXPECT_EQ(testing::internal::GetCapturedStderr(), "smash error: cd: too many arguments\n");
}

TEST(CdTests, NonExistentDirectory)
{
    ChangeDirCommand command = ChangeDirCommand("cd /test");

    testing::internal::CaptureStderr();
    command.execute();

    EXPECT_EQ(testing::internal::GetCapturedStderr(), "smash error: chdir failed: No such file or directory\n");
}

TEST(CdTests, EmptyString)
{
    ChangeDirCommand command = ChangeDirCommand("cd");

    testing::internal::CaptureStderr();
    // command.setOutputStream(&out);
    command.execute();
    // TODO: is this the correct output?
    // EXPECT_EQ(out.str(), "smash error: cd: too few arguments\n");
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
    testing::internal::CaptureStdout();
    command.execute();

    // compare by line

    std::string actual = testing::internal::GetCapturedStdout();
    std::string expected;
    // TODO: print the actual command received or the name?
    expected += "[1]sleep : " + std::to_string(command1->getPid()) + " 0 secs\n";
    expected += "[2]sleep : " + std::to_string(command2->getPid()) + " 0 secs\n";
    expected += "[3]sleep : " + std::to_string(command3->getPid()) + " 0 secs\n";

    EXPECT_EQ(expected, actual);
    SmallShell::getInstance().getJobsList().killAllJobs();
}

TEST(JobsTests, IgnoreArgs)
{
    JobsCommand command = JobsCommand("jobs arg1 arg2 arg3");
    std::ostringstream out;
    testing::internal::CaptureStdout();
    command.execute();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
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
    testing::internal::CaptureStdout();

    command.execute();

    // compare by line

    std::string actual = testing::internal::GetCapturedStdout();
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
    testing::internal::CaptureStdout();

    command.execute();

    // compare by line

    std::string actual = testing::internal::GetCapturedStdout();
    std::string expected;

    expected += "sleep : " + std::to_string(command1->getPid()) + "\n";

    EXPECT_EQ(expected, actual);
    SmallShell::getInstance().getJobsList().killAllJobs();
}

TEST(FgTests, StoppedJob)
{
    SmallShell::getInstance().getJobsList().killAllJobs();
    ExternalCommand *command1 = new ExternalCommand("sleep 2 &");
    command1->execute();

    SmallShell::getInstance().getJobsList().getJobById(1).cmd->stopProcess();

    ForegroundCommand command = ForegroundCommand("fg 1");
    testing::internal::CaptureStdout();

    command.execute();

    // compare by line

    std::string actual = testing::internal::GetCapturedStdout();
    std::string expected;

    expected += "sleep : " + std::to_string(command1->getPid()) + "\n";

    EXPECT_EQ(expected, actual);
    SmallShell::getInstance().getJobsList().killAllJobs();
}

TEST(FgTests, InvalidId)
{
    ForegroundCommand command = ForegroundCommand("fg 1");
    testing::internal::CaptureStderr();
    command.execute();

    EXPECT_EQ(testing::internal::GetCapturedStderr(), "smash error: fg: job-id 1 does not exist\n");
}

TEST(FgTests, TooManyArgs)
{
    ForegroundCommand command = ForegroundCommand("fg 1 2 3");
    testing::internal::CaptureStderr();
    command.execute();

    EXPECT_EQ(testing::internal::GetCapturedStderr(), "smash error: fg: invalid arguments\n");
}

// Test bg command

// TODO: test bg with stopped job

// Test quit command

TEST(QuitTests, BasicQuit)
{
    QuitCommand command = QuitCommand("quit");
    testing::internal::CaptureStdout();
    command.execute();

    EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
}

TEST(QuitTests, IgnoreArgs)
{
    QuitCommand command = QuitCommand("quit arg1 arg2 arg3");
    testing::internal::CaptureStdout();
    command.execute();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
}

TEST(QuitTests, QuitKill)
{
    ExternalCommand command1 = ExternalCommand("sleep 2 &");
    command1.execute();

    QuitCommand command = QuitCommand("quit kill");
    testing::internal::CaptureStdout();
    command.execute();

    EXPECT_EQ(testing::internal::GetCapturedStdout(), "smash: sending SIGKILL signal to 1 jobs:\n" + std::to_string(command1.getPid()) + ": sleep 2 &\n");
}

TEST(PipeTests, ExternalToExternal)
{
    testing::internal::CaptureStdout();

    SmallShell::getInstance().executeCommand("cat /etc/passwd | grep root");

    std::string actual = testing::internal::GetCapturedStdout();

    EXPECT_EQ(actual, "root:x:0:0:root:/root:/bin/bash\n");
}

TEST(PipeTests, ExternalToInternal)
{
    testing::internal::CaptureStdout();

    SmallShell::getInstance().executeCommand("cat /etc/passwd | quit");

    std::string actual = testing::internal::GetCapturedStdout();

    EXPECT_EQ(actual, "");
}

TEST(PipeTests, InternalToExternal)
{
    testing::internal::CaptureStdout();

    SmallShell::getInstance().executeCommand("jobs | grep root");

    std::string actual = testing::internal::GetCapturedStdout();

    EXPECT_EQ(actual, "");
}

TEST(PipeTests, InternalToInternal)
{
    testing::internal::CaptureStdout();

    SmallShell::getInstance().executeCommand("jobs | quit");

    std::string actual = testing::internal::GetCapturedStdout();

    EXPECT_EQ(actual, "");
}

TEST(PipeTests, PipeToPipe)
{
    testing::internal::CaptureStdout();

    SmallShell::getInstance().executeCommand("jobs | grep root | quit");

    std::string actual = testing::internal::GetCapturedStdout();

    EXPECT_EQ(actual, "");
}

TEST(PipeTests, AmpersandErrPipe)
{
    testing::internal::CaptureStderr();

    SmallShell::getInstance().executeCommand("fg 1 2 3 |& grep root");

    std::string actual = testing::internal::GetCapturedStderr();

    EXPECT_EQ(actual, "smash error: fg: invalid arguments\n");
}
//test for getfiletype
TEST(FileTypeTests, BasicFileType)
{
    testing::internal::CaptureStdout();
    //for each file type
    SmallShell::getInstance().executeCommand("getfiletype /etc/passwd");
    std::string actual = testing::internal::GetCapturedStdout();

    std::string expected = "/etc/passwd's type is \"regular file\" and takes";

    // trim actual size
    actual = actual.substr(0, actual.find(" up"));

    EXPECT_EQ(actual, expected);

}

TEST(FileTypeTests, FileTypeDir)
{
    testing::internal::CaptureStdout();
    // for each file type
    SmallShell::getInstance().executeCommand("getfiletype /etc");
    std::string actual = testing::internal::GetCapturedStdout();

    std::string expected = "/etc's type is \"directory\" and takes";

    // trim actual size
    actual = actual.substr(0, actual.find(" up"));

    EXPECT_EQ(actual, expected);
}

TEST(ChangeFileModeCommand, BasicChangeFileMode)
{
    testing::internal::CaptureStderr();
    // for each file type
    SmallShell::getInstance().executeCommand("chmod 777 /etc/passwd");
    std::string actual = testing::internal::GetCapturedStderr();

    std::string expected = "smash error: chmod failed: Operation not permitted\n";

    EXPECT_EQ(actual, expected);
}

TEST(ChangeFileModeCommand, ChangeFileModeInvalidArgs)
{
    testing::internal::CaptureStderr();
    // for each file type
    SmallShell::getInstance().executeCommand("chmod 777 /etc/passwd 123");
    std::string actual = testing::internal::GetCapturedStderr();

    std::string expected = "smash error: chmod: invalid arguments\n";

    EXPECT_EQ(actual, expected);
}

TEST(ChangeFileModeCommand, ChangeFileModeInvalidFile)
{
    testing::internal::CaptureStderr();
    // for each file type
    SmallShell::getInstance().executeCommand("chmod 777 /etc/passwd123");
    std::string actual = testing::internal::GetCapturedStderr();

    std::string expected = "smash error: chmod failed: No such file or directory\n";

    EXPECT_EQ(actual, expected);
}

TEST(ChangeFileModeCommand, ChangeFileModeInvalidMode)
{
    testing::internal::CaptureStderr();
    // for each file type
    SmallShell::getInstance().executeCommand("chmod 7777 /etc/passwd");
    std::string actual = testing::internal::GetCapturedStderr();

    std::string expected = "smash error: chmod failed: Operation not permitted\n";

    EXPECT_EQ(actual, expected);
}

TEST(ChangeFileModeCommand, CreateAndChange)
{
    testing::internal::CaptureStdout();
    // for each file type
    SmallShell::getInstance().executeCommand("touch test.txt");
    SmallShell::getInstance().executeCommand("chmod 777 test.txt");
    std::string actual = testing::internal::GetCapturedStdout();

    std::string expected = "";

    EXPECT_EQ(actual, expected);
}

TEST(SetCoreCommand, FewArguments)
{
    testing::internal::CaptureStderr();
    // for each file type
    SmallShell::getInstance().executeCommand("setcore 1");
    std::string actual = testing::internal::GetCapturedStderr();

    std::string expected = "smash error: setcore: invalid arguments\n";

    EXPECT_EQ(actual, expected);
}

TEST(SetCoreCommand, InvalidJob)
{
    testing::internal::CaptureStderr();
    // for each file type
    SmallShell::getInstance().executeCommand("setcore 0 1");
    std::string actual = testing::internal::GetCapturedStderr();

    std::string expected = "smash error: setcore: job-id 0 does not exist\n";

    EXPECT_EQ(actual, expected);
}

TEST(SetCoreCommand, BasicSetCore)
{
    // create nano process in the background
    ExternalCommand command = ExternalCommand("nano &");
    command.execute();

    testing::internal::CaptureStdout();

    // set core to 1
    SmallShell::getInstance().executeCommand("setcore 1 1");
    std::string actual = testing::internal::GetCapturedStdout();

    std::string expected = "";

    EXPECT_EQ(actual, expected);

    // check if nano is running on core 1 using ps
    testing::internal::CaptureStdout();

    // format should be
    std::string cmd = "ps -o pid,psr,comm -p " + std::to_string(command.getPid()) + " | grep nano";
    SmallShell::getInstance().executeCommand(cmd.c_str());

    expected = std::to_string(command.getPid()) + "   1 nano\n";

    // set core to 2
    actual = testing::internal::GetCapturedStdout();
    EXPECT_EQ(actual, expected);

    // check if nano is running on core 2 using ps
    testing::internal::CaptureStdout();
    SmallShell::getInstance().executeCommand("setcore 1 2");

    // sleep for 0.1 seconds to make sure the process is running on core 2
    sleep(0.1);

    SmallShell::getInstance().executeCommand(cmd.c_str());

    expected = std::to_string(command.getPid()) + "   2 nano\n";

    actual = testing::internal::GetCapturedStdout();

    EXPECT_EQ(actual, expected);
}