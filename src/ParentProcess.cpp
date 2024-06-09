#include <sys/wait.h>

#include "CustomException.h"
#include "ParentProcess.h"

ParentProcess::ParentProcess() {
  struct sigaction sa;
  sa.sa_handler = &ParentProcess::handleChildSignal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  if (sigaction(SIGCHLD, &sa, 0) == -1) {
    throw CustomException("Failed to set signal handler");
  }
}

void ParentProcess::handleChildSignal(int signal) {
  pid_t childPid = wait(nullptr);
  std::exit(EXIT_FAILURE);
}