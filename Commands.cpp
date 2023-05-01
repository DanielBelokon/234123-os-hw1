#include "Commands.h"

// TODO: Add your implementation for classes in Commands.h

void Command::setIODescriptors()
{
    // save the old descriptors
    oldStdout = dup(STDOUT_FILENO);
    oldStderr = dup(STDERR_FILENO);
    oldStdin = dup(STDIN_FILENO);

    // set the new descriptors
    if (output_fd != -1)
    {
        dup2(output_fd, STDOUT_FILENO);
    }
    if (error_fd != -1)
    {
        dup2(error_fd, STDERR_FILENO);
    }
    if (input_fd != -1)
    {
        dup2(input_fd, STDIN_FILENO);
    }
}

void Command::cleanup()
{
    // restore the old descriptors
    if (output_fd != -1)
    {
        close(output_fd);
    }

    if (error_fd != -1)
    {
        close(error_fd);
    }

    if (input_fd != -1)
    {
        close(input_fd);
    }

    dup2(oldStdout, STDOUT_FILENO);
    dup2(oldStderr, STDERR_FILENO);
    dup2(oldStdin, STDIN_FILENO);
}

std::string Command::getCommandName() const
{
    return cmd_v[0];
}

PipeCommand::PipeCommand(const char *cmd_line, std::vector<Command *> commands, std::vector<bool> redirectErr) : Command(cmd_line), commands(commands), redirectErrVector(redirectErr)
{
    // create pipes for each command and set the fd member appropriately
    for (int i = 0; i < commands.size() - 1; i++)
    {
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("smash error: pipe failed");
            return;
        }
        commands[i + 1]->input_fd = pipefd[0];

        if (redirectErr[i])
        {
            commands[i]->error_fd = pipefd[1];
        }
        else
        {
            commands[i]->output_fd = pipefd[1];
        }
    }
}

void PipeCommand::execute()
{
    // execute each command in the pipe
    for (int i = 0; i < commands.size(); i++)
    {
        commands[i]->setIODescriptors();
        commands[i]->execute();
        commands[i]->cleanup();
    }
}
