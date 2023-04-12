#include "Commands.h"

class ExternalCommand : public Command
{
    int pid;

public:
    ExternalCommand(const char *cmd_line);
    virtual ~ExternalCommand() {}
    void execute() override;
    int getPid();
};