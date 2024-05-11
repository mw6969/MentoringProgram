#ifndef _TOOL_
#define _TOOL_

#include <string>

#include "SharedMemory.h"

class Tool {
public:
  Tool() = default;
  ~Tool() = default;

  void reader(const std::string &inputFile);
  void writer(const std::string &outputFile);

private:
  SharedMemory sharedMemory_;
};

#endif