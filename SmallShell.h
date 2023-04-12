#pragma once

#include "BuiltInCommands.h"
#include "ExternalCommands.h"
// #include "PipeCommand.h"
// #include "RedirectionCommand.h"
#include <string>
#include <linux/limits.h>
#include "CommandUtils.h"

class SmallShell
{
private:
    std::string prompt = "smash> ";
    int PID = 0;
    // TODO: Add your data members
    SmallShell();

public:
    Command *CreateCommand(const char *cmd_line);
    SmallShell(SmallShell const &) = delete;     // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator
    int getPID();
    std::string getPWD();
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
    ~SmallShell();
    void executeCommand(const char *cmd_line);
    // TODO: add extra methods as needed
};