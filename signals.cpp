#include "signals.h"
#include "Commands.h"
#include "SmallShell.h"

void ctrlZHandler(int sig_num)
{
  std::cout << "smash: got ctrl-Z" << std::endl;
  SmallShell &smash = SmallShell::getInstance();
  smash.ctrlZHandler();
}

void ctrlCHandler(int sig_num)
{
  std::cout << "smash: got ctrl-C" << std::endl;
  SmallShell &smash = SmallShell::getInstance();
  smash.ctrlCHandler();
}

void alarmHandler(int sig_num)
{
  std::cout << "smash: got an alarm" << std::endl;
  SmallShell &smash = SmallShell::getInstance();
  smash.alarmHandler();
}
