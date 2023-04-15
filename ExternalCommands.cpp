#include "ExternalCommands.h"
#include "SmallShell.h"

ExternalCommand::ExternalCommand(const char *cmd_line) : Command(cmd_line)
{
    this->_executeInBackground = CommandUtils::_isBackgroundComamnd(cmd_line);
}

void ExternalCommand::execute()
{
    // Create a forked external process
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
    // Parent process
    else
    {
        if (this->_executeInBackground)
        {
            // add to jobs list
            SmallShell::getInstance().getJobsList().addJob(this);
        }
        else
        {
            // wait for child process to finish
            waitpid(pid, NULL, WUNTRACED);
        }
    }
}

int ExternalCommand::getPid()
{
    return pid;
}
void ExternalCommand::killProcess(){
    // to do : implement.
    return ;
}

bool ExternalCommand::isExecuteInBackground()
{
    return _executeInBackground;
}