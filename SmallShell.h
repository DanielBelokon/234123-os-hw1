#pragma once

#include "BuiltInCommands.h"
#include "ExternalCommands.h"
// #include "PipeCommand.h"
// #include "RedirectionCommand.h"
#include <string>
#include <linux/limits.h>
#include "CommandUtils.h"
#include "JobsList.h"
class SmallShell
{
private:
    std::string prompt = "smash> ";
    int PID = 0;
    std::string prevPath;
    // TODO: Add foreground job and jobs list
    JobsList* _jobsList;
    SmallShell();

public:
    ~SmallShell();

    SmallShell(SmallShell const &) = delete;     // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator

    void executeCommand(const char *cmd_line);
    Command *CreateCommand(const char *cmd_line);

    int getPid();
    std::string getWorkingDir();
    JobsList* getJobsList();
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
};