#include "ExternalCommands.h"

void ExternalCommand::execute()
{
    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(cmd_v[0].c_str(), (char *const *)cmd_v.data()) < 0)
        {
            // TODO: Handle error, check required message
            perror("smash error: >");
            exit(1);
        }
    }
    else if (pid < 0)
    {
        // TODO: Handle error, check required message
        perror("smash error: >");
    }
    else
    {
        // Parent process - check if foreground or background
        if (cmd_v[cmd_v.size() - 1] == "&")
        {
            // TODO: Add to jobs list
        }
        else
        {
            // TODO: Foreground process through SmallShell function
        }
    }
}

int ExternalCommand::getPid()
{
    return pid;
}