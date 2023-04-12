#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <iostream>
#include <cstdarg>
#include "CommandUtils.h"

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class Command
{
  // TODO: Add your data members
  // output stream for the command
  std::ostream *out;

protected:
  std::vector<std::string> cmd_v;

public:
  Command(const char *cmd_line)
  {
    cmd_v = CommandUtils::_split(cmd_line, ' ');
  }
  virtual ~Command(){};
  virtual void execute() = 0;
  std::string getCommandName() const ;
  void setOutputStream(std::ostream *out)
  {
    this->out = out;
  }

  std::ostream &getOutputStream()
  {
    if (out != nullptr)
    {
      return *out;
    }
    else // default output stream
    {
      return std::cout;
    }
  }

  // virtual void prepare();
  // virtual void cleanup();
  //  TODO: Add your extra methods if needed
};

class PipeCommand : public Command
{
  // TODO: Add your data members
public:
  PipeCommand(const char *cmd_line);
  virtual ~PipeCommand() {}
  void execute() override;
};

class RedirectionCommand : public Command
{
  // TODO: Add your data members
public:
  explicit RedirectionCommand(const char *cmd_line);
  virtual ~RedirectionCommand() {}
  void execute() override;
  // void prepare() override;
  // void cleanup() override;
};

#endif // SMASH_COMMAND_H_
