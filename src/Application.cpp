#include <unistd.h>

#include "Application.h"
#include "ChildProcess.h"
#include "CustomException.h"
#include "ParentProcess.h"

Application::Application(const std::string &inputFile,
                         const std::string &outputFile) {
  if (pid_t pid = fork(); pid == -1) {
    throw CustomException("Failed to fork");
  } else if (pid > 0) {
    ParentProcess parent(inputFile);
  } else {
    ChildProcess child(outputFile);
  }
}