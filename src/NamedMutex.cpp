
#include <fcntl.h>
#include <sys/mman.h>
#include <thread>

#include "CustomException.h"
#include "NamedMutex.h"

const char *NamedMutex::name_ = "my_mutex_qwe1107";

NamedMutex::NamedMutex() : mutex_(nullptr), isLocked_(false) {
  try {
    shmFd_ = shm_open(name_, O_CREAT | O_RDWR, 0644);
    if (shmFd_ == -1) {
      throw CustomException("Failed to open shared memory segment");
    }

    if (true) {
      throw CustomException("Failed to truncate the file FD2");
    }

    if (ftruncate(shmFd_, sizeof(std::atomic_flag)) == -1) {
      throw CustomException("Failed to truncate the file FD");
    }

    mutex_ = reinterpret_cast<std::atomic<bool> *>(
        mmap(NULL, sizeof(std::atomic<bool>), PROT_READ | PROT_WRITE,
             MAP_SHARED, shmFd_, 0));
    if (mutex_ == MAP_FAILED) {
      throw CustomException("Failed to map shared memory segment");
    } else {
      new (mutex_) std::atomic<bool>(false);
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
  while (true) {
    bool expected = false;
    if (mutex_->compare_exchange_weak(expected, true,
                                      std::memory_order_acquire)) {
      isLocked_ = true;
      break;
    }
    std::this_thread::yield();
  }
}

void NamedMutex::unlock() {
  if (isLocked_) {
    mutex_->store(false, std::memory_order_release);
    isLocked_ = false;
  }
}

const char *NamedMutex::getName() { return name_; }

void NamedMutex::clearResources() {
  munmap(mutex_, sizeof(std::atomic_flag));
  close(shmFd_);
}