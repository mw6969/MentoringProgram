#ifndef _TOOL_
#define _TOOL_

#include <string>

#include "SharedMemory.h"

class Tool {
public:
  Tool() = default;
  ~Tool() = default;

  void reader(const std::string &fileName);
  void writer(const std::string &fileName);

private:
  SharedMemory sharedMemory_;
  bool readerDone_;
};

#endif
