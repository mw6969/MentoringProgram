
#include <fcntl.h>
#include <sys/mman.h>
#include <thread>

#include "CustomException.h"
#include "NamedMutex.h"

const char *NamedMutex::name_ = "my_mutex_qwe1104";

NamedMutex::NamedMutex() : mutex_(nullptr) {
  try {
    shmFd_ = shm_open(name_, O_CREAT | O_RDWR, 0644);
    if (shmFd_ == -1) {
      throw CustomException("Failed to open shared memory segment");
    }

    if (ftruncate(shmFd_, sizeof(std::atomic_flag)) == -1) {
      throw CustomException("Failed to truncate the file FD");
    }

    mutex_ = reinterpret_cast<std::atomic_flag *>(
        mmap(NULL, sizeof(std::atomic_flag), PROT_READ | PROT_WRITE, MAP_SHARED,
             shmFd_, 0));
    if (mutex_ == MAP_FAILED) {
      throw CustomException("Failed to map shared memory segment");
    }
  } catch (...) {
    clearResources();
    if (auto eptr = std::current_exception(); eptr) {
      std::rethrow_exception(eptr);
    }
  }
}

NamedMutex::~NamedMutex() { clearResources(); }

void NamedMutex::lock() {
  while (mutex_->test_and_set()) {
    std::this_thread::yield();
  }
}

void NamedMutex::unlock() { mutex_->clear(); }

const char *NamedMutex::getName() { return name_; }

void NamedMutex::clearResources() {
  munmap(mutex_, sizeof(std::atomic_flag));
  close(shmFd_);
}