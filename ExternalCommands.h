#include "Commands.h"
#pragma once
class ExternalCommand : public Command
{
    std::string cmd_line;
    bool _executeInBackground;
    time_t timeout = -1;

public:
    ExternalCommand(const char *cmd_line, time_t timeout = -1);
    virtual ~ExternalCommand() {}
    void execute() override;
    bool isExecuteInBackground();
};