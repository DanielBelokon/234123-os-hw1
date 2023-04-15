#include "Commands.h"
#pragma once
class ExternalCommand : public Command
{
    int pid;

public:
    ExternalCommand(const char *cmd_line);
    virtual ~ExternalCommand() {}
    void execute() override;
    int getPid();

    // SIGCONT process
    void continueProcess();
    // SIGSTOP process
    void stopProcess();
    // SIGKILL process
    void killProcess();
};