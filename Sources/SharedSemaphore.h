#ifndef _SHAREDSEMAPHORE_
#define _SHAREDSEMAPHORE_

#include <stdio.h>

class SharedSemaphore {
public:
  SharedSemaphore();
  ~SharedSemaphore();

private:
  char fileName[FILENAME_MAX];
  int fileDescriptor;
};

#endif