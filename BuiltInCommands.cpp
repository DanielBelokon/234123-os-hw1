#include "BuiltInCommands.h"
#pragma region ChangePromptCommand

/// @brief Change the prompt of the shell
void ChangePromptCommand::execute()
{
    if (cmd_v.size() > 1)
    {
        prompt = cmd_v[1];
    }
    else
    {
        prompt = "smash";
    }
    SmallShell::getInstance().setPrompt(prompt);
}

#pragma endregion

#pragma region ShowPidCommand
ShowPidCommand::ShowPidCommand(const char *cmd_line) : BuiltInCommand(cmd_line)
{
}

/// @brief Show the PID of the shell
void ShowPidCommand::execute()
{
    this->getOutputStream() << "smash pid is " << SmallShell::getInstance().getPid() << std::endl;
}
#pragma endregion

#pragma region ChangeDirCommand
ChangeDirCommand::ChangeDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line)
{
}

/// @brief Change the current directory of the shell
void ChangeDirCommand::execute()
{
    if (cmd_v.size() == 2)
    {
        path = cmd_v[1];
    }
    else if (cmd_v.size() > 2)
    {
        this->getOutputStream() << "smash error: cd: too many arguments" << std::endl;
    }

    if (path == "-")
    {
        if (SmallShell::getInstance().getPrevPath() == "")
        {
            this->getOutputStream() << "smash error: cd: OLDPWD not set" << std::endl;
            return;
        }

        path = SmallShell::getInstance().getPrevPath();
    }

    std::string currPath = SmallShell::getInstance().getWorkingDir();

    if (chdir(path.c_str()) == -1)
    {
        perror("smash error: chdir failed");
    }
    else
    {
        SmallShell::getInstance().setPrevPath(currPath);
    }
}

#pragma endregion

#pragma region GetCurrDirCommand
GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line)
{
}

void GetCurrDirCommand::execute()
{
    this->getOutputStream() << SmallShell::getInstance().getWorkingDir() << std::endl;
}
#pragma endregion

#pragma region JobsCommand

JobsCommand::JobsCommand(const char *cmd_line) : BuiltInCommand(cmd_line)
{
}

void JobsCommand::execute()
{
    SmallShell::getInstance().getJobsList().printJobsList();
}

#pragma endregion

#pragma region ForegroundCommand
ForegroundCommand::ForegroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line)
{
}

void ForegroundCommand::execute()
{
    // check if there is any id with the command
    if (cmd_v.size() > 2)
    {
        this->getOutputStream() << "smash error: fg: invalid arguments" << std::endl;
    }

    if (cmd_v.size() == 1)
    {
        JobsList::JobEntry job = SmallShell::getInstance().getJobsList().getJobWithMaxID();
        MoveJobToForeground(job);
    }
    else{
        int id = atoi(cmd_v[1].c_str());
        JobsList::JobEntry job = SmallShell::getInstance().getJobsList().getJobById(id);
        MoveJobToForeground(job);
    }
}
void ForegroundCommand::MoveJobToForeground(JobsList::JobEntry &job)
{
    // print the command with PID
    this->getOutputStream() << cmd_v[0] << " : " << job.getJobPid() << std::endl;
    // sent job to foreground
    SmallShell::getInstance().getJobsList().removeJobById(job.getJobId());
    kill(job.getJobPid(), SIGCONT);
    waitpid(job.getJobPid(), nullptr, WUNTRACED);
}
#pragma endregion
