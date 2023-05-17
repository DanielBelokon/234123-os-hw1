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
    else if (firstWord.compare("getfiletype") == 0)
    {
        return new GetFileInfoCommand(cmd_line);
    }
    else if (firstWord.compare("chmod") == 0)
    {
        return new ChangeFileModeCommand(cmd_line);
    }
    else if (firstWord.compare("setcore") == 0)
    {
        return new SetCoreCommand(cmd_line);
    }
    else if (cmd_s.find('*') != std::string::npos || cmd_s.find('?') != std::string::npos)
    {
        std::string bash_cmd = "bash -c \"";
        bash_cmd += cmd_s;
        bash_cmd += "\"";
        return new ExternalCommand(bash_cmd.c_str());
    }
    else if (firstWord.compare("") != 0)
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

    // handle timeout (this will of course be refactored nicely :)
    if (cmd_s.find("timeout") == 0)
    {
        // split by space
        std::vector<std::string> cmd_v = _split(cmd_s, ' ');

        // check if timeout is valid
        if (cmd_v.size() < 3)
        {
            std::cerr << "smash error: timeout: invalid arguments" << std::endl;
            return;
        }

        // get timeout
        std::string timeout_s = cmd_v[1];

        // create external command out of the rest of the command
        std::string timeout_cmd = "";
        for (int i = 2; i < cmd_v.size(); i++)
        {
            timeout_cmd += cmd_v[i];
            timeout_cmd += " ";
        }

        ExternalCommand command = ExternalCommand(timeout_cmd.c_str(), std::stoi(timeout_s));

        command.setIODescriptors();
        command.execute();
        command.cleanup();
        return;
    }

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

        // foreach command

        for (Command *cmd : commands)
        {
            delete cmd;
        }

        return;
    }

    Command *cmd = CreateCommand(cmd_line);
    if (cmd == nullptr)
    {
        return;
    }

    cmd->setIODescriptors();
    cmd->execute();
    cmd->cleanup();
    delete cmd;
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
