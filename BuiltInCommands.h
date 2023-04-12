#pragma once

#include "Commands.h"
#include "SmallShell.h"
#include "CommandUtils.h"
#include <string>

class BuiltInCommand : public Command
{

public:
    BuiltInCommand(const char *cmd_line) : Command(cmd_line)
    {
    }
    virtual ~BuiltInCommand() {}
};

class ChangeDirCommand : public BuiltInCommand
{
public:
    std::string path;

public:
    ChangeDirCommand(const char *cmd_line);
    virtual ~ChangeDirCommand() {}
    void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand
{
public:
    GetCurrDirCommand(const char *cmd_line);
    virtual ~GetCurrDirCommand() {}
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand
{
public:
    ShowPidCommand(const char *cmd_line);
    virtual ~ShowPidCommand() {}
    void execute() override;
};

class ChangePromptCommand : public BuiltInCommand
{
    std::string prompt;

public:
    ChangePromptCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}
    // ChangePromptCommand(const std::vector<std::string> &cmd_v);
    virtual ~ChangePromptCommand() {}
    void execute() override;
};

class JobsList;
class QuitCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    QuitCommand(const char *cmd_line, JobsList *jobs);
    virtual ~QuitCommand() {}
    void execute() override;
};

class JobsCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    JobsCommand(const char *cmd_line, JobsList *jobs);
    virtual ~JobsCommand() {}
    void execute() override;
};

class ForegroundCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    ForegroundCommand(const char *cmd_line, JobsList *jobs);
    virtual ~ForegroundCommand() {}
    void execute() override;
};

class BackgroundCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    BackgroundCommand(const char *cmd_line, JobsList *jobs);
    virtual ~BackgroundCommand() {}
    void execute() override;
};

class TimeoutCommand : public BuiltInCommand
{
    /* Bonus */
    // TODO: Add your data members
public:
    explicit TimeoutCommand(const char *cmd_line);
    virtual ~TimeoutCommand() {}
    void execute() override;
};

class ChmodCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    ChmodCommand(const char *cmd_line);
    virtual ~ChmodCommand() {}
    void execute() override;
};

class GetFileTypeCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    GetFileTypeCommand(const char *cmd_line);
    virtual ~GetFileTypeCommand() {}
    void execute() override;
};

class SetcoreCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    SetcoreCommand(const char *cmd_line);
    virtual ~SetcoreCommand() {}
    void execute() override;
};

class KillCommand : public BuiltInCommand
{
    // TODO: Add your data members
public:
    KillCommand(const char *cmd_line, JobsList *jobs);
    virtual ~KillCommand() {}
    void execute() override;
};