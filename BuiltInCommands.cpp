#include "BuiltInCommands.h"

#pragma region ChangePromptCommand
ChangePromptCommand::ChangePromptCommand(const char *cmd_line) : BuiltInCommand(cmd_line)
{
    prompt = std::string(cmd_line).substr(9);
}

/// @brief Change the prompt of the shell
void ChangePromptCommand::execute()
{
    SmallShell::getInstance().setPrompt(prompt);
}

#pragma endregion

#pragma region ShowPidCommand
ShowPidCommand::ShowPidCommand(const char *cmd_line) : BuiltInCommand(cmd_line)
{
}

/// @brief Show the PID of the shell
void ShowPidCommand::execute()
{
    std::cout << "smash pid is " << SmallShell::getInstance().getPID() << std::endl;
}
#pragma endregion

#pragma region ChangeDirCommand
ChangeDirCommand::ChangeDirCommand(const char *cmd_line, char **plastPwd) : BuiltInCommand(cmd_line)
{
}

/// @brief Change the current directory of the shell
void ChangeDirCommand::execute()
{
}

#pragma endregion
