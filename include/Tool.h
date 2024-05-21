#ifndef _TOOL_
#define _TOOL_

#include <string>

#include "Logger.h"
#include "SharedMemory.h"

class Tool {
public:
  Tool(Logger &logger);
  ~Tool() = default;

  void reader(const std::string &inputFile);
  void writer(const std::string &outputFile);

private:
  SharedMemory sharedMemory_;
  Logger &logger_;
};

#endif