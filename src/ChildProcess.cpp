#include <cstdlib>
#include <signal.h>
#include <sys/prctl.h>

#include "ChildProcess.h"

ChildProcess::ChildProcess() {
  prctl(PR_SET_PDEATHSIG, SIGTERM);
  signal(SIGTERM, ChildProcess::handleParentSignal);
}

void ChildProcess::handleParentSignal(int signal) {
  if (signal == SIGTERM) {
    std::exit(EXIT_FAILURE);
  }
}