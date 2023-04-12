#include "Commands.h"

// TODO: Add your implementation for classes in Commands.h
Command::Command(const char *cmd_line)
{
    _firstWordOfCommand = cmd_line;
}

Command::~Command()
{
}
std::string Command::getCommand() const
{
    std::string firstWord = firstWordOfCommand.substr(0, firstWordOfCommand.find_first_of(" \n"));
    reutrn firsrWord;
}
