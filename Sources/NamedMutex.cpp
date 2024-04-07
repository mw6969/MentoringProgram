
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <thread>

#include "NamedMutex.h"

NamedMutex::NamedMutex(const char *mutexName) : mutex_(nullptr) {
  // Create or open the shared memory region
  shmFd_ = shm_open(mutexName, O_CREAT | O_RDWR, 0644);
  if (shmFd_ == -1) {
    std::cerr << "Failed to create/open shared memory: " << strerror(errno)
              << std::endl;
    exit(1);
  }

  // Resize the shared memory region to hold the mutex
  if (ftruncate(shmFd_, sizeof(std::atomic_flag)) == -1) {
    std::cerr << "Failed to resize shared memory: " << strerror(errno)
              << std::endl;
    exit(1);
  }

  // Map the shared memory region into the process' address space
  mutex_ = reinterpret_cast<std::atomic_flag *>(
      mmap(NULL, sizeof(std::atomic_flag), PROT_READ | PROT_WRITE, MAP_SHARED,
           shmFd_, 0));
  if (mutex_ == MAP_FAILED) {
    std::cerr << "Failed to map shared memory: " << strerror(errno)
              << std::endl;
    exit(1);
  }
}

NamedMutex::~NamedMutex() {
  // Unmap the shared memory region
  munmap(mutex_, sizeof(std::atomic_flag));

  // Close the shared memory file descriptor
  close(shmFd_);
}

void NamedMutex::lock() {
  // Spin until the lock is acquired
  while (mutex_->test_and_set()) {
    std::this_thread::yield();
  }
}

void NamedMutex::unlock() { mutex_->clear(); }