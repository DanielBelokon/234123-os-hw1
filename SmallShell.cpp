#include "SmallShell.h"

using namespace CommandUtils;

SmallShell::SmallShell()
{
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
    std::vector<std::string> cmd_v = _split(cmd_s);
    std::string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));
    // TODO: create a factory class that will create the commands, map between command name and command class
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
    else if (firstWord.compare("jobs") == 0)
    {
        return new JobsCommand(cmd_line);
    }
    else if (firstWord.compare("bg") == 0)
    {
        return new BackgroundCommand(cmd_line);
    }
    else if (firstWord.compare("cd") == 0)
    {
        return new ChangeDirCommand(cmd_line);
    }
    else if (firstWord.compare("fg") == 0)
    {
        return new ForegroundCommand(cmd_line);
    }
    else if (firstWord.compare("quit") == 0)
    {
        return new QuitCommand(cmd_line);
    }
    else if (firstWord.compare("kill") == 0)
    {
        return new KillCommand(cmd_line);
    }
    else if (cmd_s.find('*') != std::string::npos || cmd_s.find('?') != std::string::npos)
    {
        std::string bash_cmd = "bash -c \"";
        bash_cmd += cmd_s;
        bash_cmd += "\"";
        return new ExternalCommand(bash_cmd.c_str());
    }
    else
    {
        return new ExternalCommand(cmd_line);
    }

    return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line)
{
    // TODO: Add your implementation here
    std::string cmd_s = _trim(std::string(cmd_line));
    // handle pipe

    if (cmd_s.find("|") != std::string::npos)
    {
        std::vector<std::string> cmd_v = _split(cmd_s, '|');
        std::vector<Command *> commands;
        std::vector<bool> RedirectErrVector = {};
        for (std::string cmd : cmd_v)
        {
            bool redirectError = false;
            // remove spaces
            cmd = _trim(cmd);
            if (cmd.empty())
            {
                continue;
            }

            if (cmd[0] == '&')
            {
                cmd = cmd.substr(1);
                cmd = _trim(cmd);
                redirectError = true;
            }
            commands.push_back(CreateCommand(cmd.c_str()));
            RedirectErrVector.push_back(redirectError);
        }

        PipeCommand pipeCommand = PipeCommand(cmd_line, commands, RedirectErrVector);
        pipeCommand.execute();
        return;
    }

    Command *cmd = CreateCommand(cmd_line);
    cmd->setIODescriptors();
    cmd->execute();
    cmd->cleanup();
    // delete cmd;
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}

int SmallShell::getPid()
{
    return getpid();
}
std::string SmallShell::getWorkingDir()
{
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    return std::string(cwd);
}
JobsList &SmallShell::getJobsList()
{
    return _jobsList;
}

