#include "SmallShell.h"

using namespace CommandUtils;

SmallShell::SmallShell()
{
    // TODO: add your implementation
}

SmallShell::~SmallShell()
{
    // TODO: add your implementation
}

/**
 * Creates and returns a pointer to Command class which matches the given command line (cmd_line)
 */
Command *SmallShell::CreateCommand(const char *cmd_line)
{
    // For example:

    std::string cmd_s = _trim(std::string(cmd_line));
    std::vector<std::string> cmd_v = _split(cmd_s, ' ');
    std::string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

    if (firstWord.compare("chprompt") == 0)
    {
        return new ChangePromptCommand(cmd_line);
    }
    else if (firstWord.compare("showpid") == 0)
    {
        return new ShowPidCommand(cmd_line);
    }
    else if (firstWord.compare("pwd") == 0)
    {
         return new GetCurrDirCommand(cmd_line);
    }
    // else if (firstWord.compare("cd") == 0)
    // {
    //     return new ChangeDirCommand(cmd_line);
    // }
    // else if (firstWord.compare("jobs") == 0)
    // {
    //     return new JobsCommand(cmd_line);
    // }
    // else if (firstWord.compare("kill") == 0)
    // {
    //     return new KillCommand(cmd_line);
    // }
    // else if (firstWord.compare("fg") == 0)
    // {
    //     return new ForegroundCommand(cmd_line);
    // }
    // else if (firstWord.compare("bg") == 0)
    // {
    //     return new BackgroundCommand(cmd_line);
    // }
    // else if (firstWord.compare("quit") == 0)
    // {
    //     return new QuitCommand(cmd_line);
    // }
    else
    {
        // return new ExternalCommand(cmd_line);
    }

    return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line)
{
    // TODO: Add your implementation here
    // for example:
    Command *cmd = CreateCommand(cmd_line);
    cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}

int SmallShell::getPid()
{
    return getpid();
}
std::string SmallShell::getWorkingDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    return std::string(cwd);
}

std::string SmallShell::getPWD()
{
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    return std::string(buf);
}