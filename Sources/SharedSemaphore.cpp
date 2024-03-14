#include <fcntl.h>
#include <unistd.h>

#include "SharedSemaphore.h"

SharedSemaphore::SharedSemaphore() {
  snprintf(fileName, sizeof(fileName), "/tmp/.%s", "myappname");

  if (fileDescriptor = open(fileName, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
      fileDescriptor != -1) {
    lockf(fileDescriptor, F_LOCK, 0);
  }
}

SharedSemaphore::~SharedSemaphore() {
  if (fileDescriptor != -1) {
    close(fileDescriptor);
    fileDescriptor = -1;
  }
}