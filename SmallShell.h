#pragma once

#include "BuiltInCommands.h"
#include "ExternalCommands.h"
// #include "PipeCommand.h"
// #include "RedirectionCommand.h"
#include <string>
#include <map>
#include <linux/limits.h>
#include "CommandUtils.h"
#include "JobsList.h"

#define NO_JOB -1

class SmallShell
{
private:
    std::string prompt = "smash> ";
    int PID = 0;
    std::string prevPath;
    int foregroundJob = NO_JOB;
    JobsList _jobsList;
    bool running = true;
    SmallShell();

public:
    ~SmallShell();

    SmallShell(SmallShell const &) = delete;     // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator

    void executeCommand(const char *cmd_line);
    Command *CreateCommand(const char *cmd_line);

    // todo: err enum
    bool ctrlZHandler()
    {
        if (foregroundJob == NO_JOB)
        {
            return false;
        }
        auto &job = SmallShell::getInstance().getJobsList().getJobById(foregroundJob);
        job.stopProcess();
        job.timeStarted = time(nullptr);

        foregroundJob = NO_JOB;
        return true;
    }

    bool ctrlCHandler()
    {
        if (foregroundJob == NO_JOB)
        {
            return false;
        }
        auto &job = SmallShell::getInstance().getJobsList().getJobById(foregroundJob);
        job.killProcess();
        foregroundJob = NO_JOB;
        return true;
    }

    bool alarmHandler()
    {
        SmallShell::getInstance().getJobsList().timeoutJob();
    }

    void setForeground(int job)
    {
        foregroundJob = job;
    }

    int
    getPid();
    std::string getWorkingDir();
    JobsList &getJobsList();

    void setPrevPath(const std::string &path)
    {
        prevPath = path;
    }

    std::string getPrevPath()
    {
        return prevPath;
    }

    std::string getPrompt()
    {
        return prompt;
    }

    void setPrompt(std::string &prompt)
    {
        this->prompt = prompt + "> ";
    }

    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    bool isRunning() const
    {
        return running;
    }

    void setRunning(bool running)
    {
        this->running = running;
    }
};