#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <iostream>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class Command
{
  // TODO: Add your data members
public:
  Command(const char *cmd_line);
  virtual ~Command();
  virtual void execute() = 0;
  std::string getCommandName() const;
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
