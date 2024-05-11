#include "ParentProcess.h"
#include "CustomException.h"
#include "Logger.h"
#include "Tool.h"

ParentProcess::ParentProcess(const std::string &inputFile) {
  sa_.sa_handler = &ParentProcess::handleSigchld;
  sigemptyset(&sa_.sa_mask);
  sa_.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  if (sigaction(SIGCHLD, &sa_, 0) == -1) {
    throw CustomException("Failed to set signal handler");
  }

  Tool tool;
  tool.reader(inputFile);
}

void ParentProcess::handleSigchld(int sig) {
  pid_t childPid = wait(nullptr);
  Logger::getInstance().getStream()
      << "Child process with pid " + std::to_string(childPid) + " crashed\n";
  std::exit(EXIT_FAILURE);
}