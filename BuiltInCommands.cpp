#include "BuiltInCommands.h"
#include "CommandUtils.h"
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
    std::cout << "smash pid is " << SmallShell::getInstance().getPid() << std::endl;
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
        this->printError("too many arguments");
        return;
    }

    if (path == "-")
    {
        if (SmallShell::getInstance().getPrevPath() == "")
        {
            this->printError("OLDPWD not set");
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
    std::cout << SmallShell::getInstance().getWorkingDir() << std::endl;
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
        this->printError("invalid arguments");
        return;
    }

    if (cmd_v.size() == 1)
    {
        if (SmallShell::getInstance().getJobsList().size() == 0)
        {
            this->printError("jobs list is empty");
            return;
        }
        JobsList::JobEntry job = SmallShell::getInstance().getJobsList().getJobWithMaxID();
        MoveJobToForeground(job);
    }
    else
    {
        if (!CommandUtils::is_digits(cmd_v[1]))
        {
            this->printError("invalid arguments");
            return;
        }

        int id = std::atoi(cmd_v[1].c_str());
        try
        {
            JobsList::JobEntry job = SmallShell::getInstance().getJobsList().getJobById(id);
            MoveJobToForeground(job);
        }
        catch (const std::exception &e)
        {
            this->printError("job-id " + cmd_v[1] + " does not exist");
        }
    }
}
void ForegroundCommand::MoveJobToForeground(JobsList::JobEntry &job)
{
    // print the command with PID
    std::cout << job.cmd << " : " << job.getPid() << std::endl;
    // sent job to foreground
    SmallShell::getInstance().setForeground(job.getJobId());
    job.continueProcess();
    waitpid(job.getPid(), nullptr, 0);
}
#pragma endregion

#pragma region BackgroundCommand

void BackgroundCommand::execute()
{
    JobsList &jlInstance = SmallShell::getInstance().getJobsList();
    // if number of arguments is not 1 or 2
    if (cmd_v.size() > 2 || !CommandUtils::is_digits(cmd_v[1]))
    {
        this->printError("invalid arguments");

        return;
    }
    // if bg alone is called and there are no jobs in the list (STOPPED)
    if (cmd_v.size() == 1 && jlInstance.countStoppedJobs() == 0)
    {
        this->printError("there is no stopped jobs to resume");
        return;
    }
    int jobId = std::stoi(cmd_v[1]);
    try
    {
        JobsList::JobEntry &job = jlInstance.getJobById(jobId);

        if (cmd_v.size() == 2 && job.getStatus() == 0)
        {
            this->printError("job-id " + cmd_v[1] + " is already running in the background");
            return;
        }
        // if the job does not exist
        if (cmd_v.size() == 2 && job.getJobId() == -1)
        {
            this->printError("job-id " + cmd_v[1] + " does not exist");
            return;
        }
    }
    catch (const std::exception &e)
    {
        this->printError("job-id " + cmd_v[1] + " does not exist");
        return;
    }

    if (cmd_v.size() > 1)
        jlInstance.continueJob(jobId);
    else
        jlInstance.getLastStoppedJob().continueProcess();
}

#pragma endregion BackgroundCommand

#pragma region QuitCommand

void QuitCommand::execute()
{
    if (cmd_v.size() > 1 && cmd_v[1] == "kill")
    {
        SmallShell::getInstance().getJobsList().killAllJobs();
    }

    SmallShell::getInstance().setRunning(false);
}

#pragma endregion QuitCommand

#pragma region KillCommand

void KillCommand::execute()
{
    if (cmd_v.size() != 3 || !CommandUtils::is_digits(cmd_v[1]) || !CommandUtils::is_digits(cmd_v[2]))
    {
        this->printError("invalid arguments");
        return;
    }

    int signal = -atoi(cmd_v[1].c_str());
    int jobId = atoi(cmd_v[2].c_str());
    JobsList::JobEntry job("", jobId, 0, 0);

    try
    {
        job = SmallShell::getInstance().getJobsList().getJobById(jobId);
    }
    catch (const std::exception &e)
    {
        this->printError("job-id " + cmd_v[2] + " does not exist");
        return;
    }
    if (job.getJobId() == -1)
    {
        this->printError("job-id " + cmd_v[2] + " does not exist");
        return;
    }

    if (kill(job.getPid(), signal) < 0)
    {
        perror("smash error: kill failed");
    }
    else
    {
        std::cout << "signal number " << signal << " was sent to pid " << job.getPid() << std::endl;
    }
}

#pragma endregion KillCommand

#pragma region SetCoreCommand

void SetCoreCommand::execute()
{
    if (cmd_v.size() != 3 || !CommandUtils::is_digits(cmd_v[1]) || !CommandUtils::is_digits(cmd_v[2]))
    {
        this->printError("invalid arguments");
        return;
    }

    int jobId = std::stoi(cmd_v[1]);
    pid_t pid = -1;
    // get job pid
    try
    {
        pid = SmallShell::getInstance().getJobsList().getJobById(jobId).getPid();
    }
    catch (const std::exception &e)
    {
        this->printError("job-id " + cmd_v[1] + " does not exist");
        return;
    }

    int core = std::stoi(cmd_v[2]);
    if (core < 0)
    {
        this->printError("invalid core number");
        return;
    }

    // set the core using sched_setaffinity
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(core, &set);
    if (sched_setaffinity(pid, sizeof(set), &set) < 0)
    {
        perror("smash error: sched_setaffinity failed");
        return;
    }
}

#pragma endregion SetCoreCommand

#pragma region ChangeFileModeCommand
void  ChangeFileModeCommand::execute()
{
    if (cmd_v.size() != 3)
    {
        this->printError("invalid arguments");
        return;
    }

    int mode = atoi(cmd_v[1].c_str());
    std::string path = cmd_v[2];

    if (chmod(path.c_str(), mode) < 0)
    {
        perror("smash error: chmod failed");
        return;
    }
}

#pragma endregion ChangeFileModeCommand