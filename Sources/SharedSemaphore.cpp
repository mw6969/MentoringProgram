#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "SharedSemaphore.h"

SharedSemaphore::SharedSemaphore() {
  // Build file name path
  snprintf(fileName, sizeof(fileName), "/tmp/.%s", "myappname");

  // Acquire critical section
  fileDescriptor = open(fileName, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  if (fileDescriptor != -1)
    lockf(fileDescriptor, F_LOCK, 0);
}

SharedSemaphore::~SharedSemaphore() {
  // Release critical section
  if (fileDescriptor != -1) {
    close(fileDescriptor);
    fileDescriptor = -1;
  }
}