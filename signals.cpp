#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlZHandler(int sig_num) {
  // TODO: send SIGTSTP to the foreground process, and add it to the jobs list
}

void ctrlCHandler(int sig_num) {
  // TODO: send SIGINT to the foreground process
}

void alarmHandler(int sig_num) {
  // TODO: Add your implementation
}

