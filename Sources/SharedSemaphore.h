#ifndef _SHAREDSEMAPHORE_
#define _SHAREDSEMAPHORE_

#include <string>

class SharedSemaphore {
public:
private:
  char fileName[FILENAME_MAX];
  int fileDescriptor;

public:
  SharedSemaphore();
  ~SharedSemaphore();

  void acquire();
  void release();
};

#endif