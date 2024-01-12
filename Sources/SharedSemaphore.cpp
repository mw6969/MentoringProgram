#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "SharedSemaphore.h"

SharedSemaphore::SharedSemaphore() {
    // Mark as not acquired
    fileDescriptor = -1;

    // Build file name path
    snprintf(fileName, sizeof(fileName), "/tmp/.%s", "myappname");
}

SharedSemaphore::~SharedSemaphore() {
    // Release critical section on object deletion
    release();
}

void SharedSemaphore::acquire() {
    // Test if section is already acquired
    if (fileDescriptor != -1)
        return;

    // Acquire critical section
    fileDescriptor = open(fileName, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR); 
    if (fileDescriptor != -1)
        lockf(fileDescriptor, F_LOCK, 0);
}

void SharedSemaphore::release() {
    // Release critical section
    if (fileDescriptor != -1) {
        close(fileDescriptor);
        fileDescriptor = -1;
    }
}