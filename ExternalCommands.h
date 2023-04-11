#include "Commands.h"

class ExternalCommand : public Command
{
public:
    ExternalCommand(const char *cmd_line);
    virtual ~ExternalCommand() {}
    void execute() override;
};