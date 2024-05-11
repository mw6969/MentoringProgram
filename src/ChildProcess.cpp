#include "ChildProcess.h"
#include "Tool.h"

ChildProcess::ChildProcess(const std::string &outputFile) {
  Tool tool;
  tool.writer(outputFile);
}