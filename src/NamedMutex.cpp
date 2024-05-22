#include <cstring>
#include <fcntl.h>
#include <thread>

#include "CustomException.h"
#include "NamedMutex.h"

const std::string name = "myMutex";

NamedMutex::NamedMutex() : mutex_(nullptr) {
  shmFd_ = shm_open(name.data(), O_CREAT | O_RDWR, 0644);
  if (shmFd_ == -1) {
    clearResources();
    throw CustomException("Failed to open shared memory segment");
  }

  if (ftruncate(shmFd_, sizeof(std::atomic_flag)) == -1) {
    clearResources();
    throw CustomException("Failed to truncate the file FD");
  }

  mutex_ = reinterpret_cast<std::atomic_flag *>(
      mmap(NULL, sizeof(std::atomic_flag), PROT_READ | PROT_WRITE, MAP_SHARED,
           shmFd_, 0));
  if (mutex_ == MAP_FAILED) {
    clearResources();
    throw CustomException("Failed to map shared memory segment");
  } else {
    new (mutex_) std::atomic_flag(false);
  }
}

NamedMutex::~NamedMutex() { clearResources(); }

void NamedMutex::lock() {
  while (mutex_->test_and_set()) {
    std::this_thread::yield();
  }
}

void NamedMutex::unlock() { mutex_->clear(); }

void NamedMutex::clearResources() {
  munmap(mutex_, sizeof(std::atomic_flag));
  shm_unlink(name.data());
  close(shmFd_);
}