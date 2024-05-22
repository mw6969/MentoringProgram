#include <iostream>
#include <unistd.h>

#include "Application.h"
#include "ChildProcess.h"
#include "CustomException.h"
#include "Logger.h"
#include "ParentProcess.h"
#include "Tool.h"

Application::Application(const std::string &inputFile,
                         const std::string &outputFile) {

  Logger &logger = Logger::getInstance();
  try {
    if (pid_t pid = fork(); pid == -1) {
      throw CustomException("Failed to fork");
    } else if (pid > 0) {
      ParentProcess parent;
      Tool tool(logger);
      tool.reader(inputFile);
    } else {
      ChildProcess child;
      Tool tool(logger);
      tool.writer(outputFile);
    }
  } catch (const CustomException &e) {
    logger.log(e.what());
  } catch (...) {
    logger.log("Caught an unknown exception\n");
  }
}