#include "SmallShell.h"
#include "signals.h"

int main(int argc, char *argv[])
{
    if (signal(SIGTSTP, ctrlZHandler) == SIG_ERR)
    {
        perror("smash error: failed to set ctrl-Z handler");
    }
    if (signal(SIGINT, ctrlCHandler) == SIG_ERR)
    {
        perror("smash error: failed to set ctrl-C handler");
    }

    struct sigaction alarm_action;
    alarm_action.sa_handler = alarmHandler;
    alarm_action.sa_flags = SA_RESTART;
    if (sigaction(SIGALRM, &alarm_action, nullptr) == -1)
    {
        perror("smash error: failed to set alarm handler");
    }

    SmallShell &smash = SmallShell::getInstance();
    while (smash.isRunning())
    {
        std::cout << smash.getPrompt();
        std::string cmd_line;
        std::getline(std::cin, cmd_line);
        smash.executeCommand(cmd_line.c_str());
    }
    return 0;
}