#include "ExternalCommands.h"
#include "SmallShell.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

ExternalCommand::ExternalCommand(const char *cmd_line) : cmd_line(cmd_line), Command(cmd_line)
{
    this->_executeInBackground = CommandUtils::_isBackgroundComamnd(cmd_line);
}

void ExternalCommand::execute()
{
    // Create a forked external process
    pid = fork();
    if (pid == 0)
    {
        // Prepare input array
        char **arg_v = new char *[cmd_v.size() + 1];

        for (int i = 0; i < cmd_v.size(); i++)
        {
            arg_v[i] = new char[cmd_v[i].length() + 1];
            strcpy(arg_v[i], cmd_v[i].c_str());
        }
        arg_v[cmd_v.size()] = NULL;

        // Child process
        setpgrp();

        if (execvp(cmd_v[0].c_str(), arg_v) == -1)
        {
            // TODO: Handle error, check required message
            perror("smash error: execvp:");
            exit(1);
        }
    }
    else if (pid < 0)
    {
        // TODO: Handle error, check required message
        perror("smash error: fork: ");
    }
    // Parent process
    else
    {
        // add to jobs list
        auto jobEntry = SmallShell::getInstance().getJobsList().addJob(pid, this->cmd_line);
        if (!this->_executeInBackground)
        {
            // wait for child process to finish
            SmallShell::getInstance().setForeground(jobEntry);
            waitpid(pid, NULL, WUNTRACED);
        }
    }
}

int ExternalCommand::getPid()
{
    return pid;
}