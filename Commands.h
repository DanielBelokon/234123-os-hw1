#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <iostream>
#include <cstdarg>
#include "CommandUtils.h"
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class Command
{
  // TODO: Add your data members
  // output stream for the command
  std::ostream *out;
  // output file descriptor

public:
  int output_fd = -1;
  int error_fd = -1;
  int input_fd = -1;

  int oldStdout;
  int oldStderr;
  int oldStdin;

protected:
  std::vector<std::string> cmd_v;

public:
  Command(const char *cmd_line)
  {
    std::string cmd_line_s = cmd_line;
    CommandUtils::_removeBackgroundSign(cmd_line_s);
    cmd_v = CommandUtils::_split(cmd_line_s, ' ');

    // handle > redirection
    std::string cmd_str = std::string(cmd_line);

    for (const auto &c : cmd_v)
    {
      if (c == ">" || c == ">>")
      {
        // pop all the arguments after the redirection
        while (cmd_v.back() != c)
        {
          cmd_v.pop_back();
        }
        cmd_v.pop_back();
      }
    }

    if (cmd_str.find(">>") != std::string::npos)
    {
      std::string filename = cmd_str.substr(cmd_str.find(">>") + 3);
      filename = filename.substr(0, filename.find_last_not_of(' ') + 1);
      this->output_fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
    }
    else if (cmd_str.find('>') != std::string::npos)
    {
      std::string filename = cmd_str.substr(cmd_str.find('>') + 2);
      filename = filename.substr(0, filename.find_last_not_of(' ') + 1);
      this->output_fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    else
    {
      this->output_fd = -1;
    }
  }

  virtual ~Command(){};
  virtual void execute() = 0;

  void setIODescriptors();
  void cleanup();

  std::string getCommandName() const;

  void printError(std::string error_msg) const
  {
    std::cerr << "smash error: " << cmd_v[0] << ": " << error_msg << std::endl;
  }

  // virtual void prepare();
  // virtual void cleanup();
  //  TODO: Add your extra methods if needed
};

class PipeCommand : public Command
{
  // TODO: Add your data members
public:
  PipeCommand(const char *cmd_line, std::vector<Command *> commands, std::vector<bool> redirectErrVector);
  std::vector<Command *> commands;
  std::vector<bool> redirectErrVector;
  virtual ~PipeCommand(){};
  void execute() override;
};

class RedirectionCommand : public Command
{
  // TODO: Add your data members
public:
  explicit RedirectionCommand(const char *cmd_line);
  virtual ~RedirectionCommand() {}
  void execute() override;
};

class GetFileInfoCommand : public Command
{
  public:
  explicit GetFileInfoCommand(const char *cmd_line);
  virtual ~GetFileInfoCommand() {}
  void execute() override;
};

#endif // SMASH_COMMAND_H_
