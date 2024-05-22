#include <assert.h>
#include <cstring>
#include <sys/mman.h>
#include <sys/shm.h>

#include "CustomException.h"
#include "SharedMemory.h"

constexpr int SharedMemoryKey = 123;

SharedMemory::SharedMemory() {
  try {
    if (id_ = shmget(SharedMemoryKey, sizeof(Buffer), IPC_CREAT | 0666);
        id_ < 0) {
      throw CustomException("Failed to get shared memory segment");
    }

    if (buffer_ = (Buffer *)shmat(id_, nullptr, 0); buffer_ == (Buffer *)(-1)) {
      throw CustomException("Failed to attach shared memory segment");
    }

    for (size_t i = 0; i < BuffersCount; ++i) {
      buffer_->freeIndexes[i] = 1;
      buffer_->writeQueue[i] = nullptr;
    }
    buffer_->readerDone = false;
  } catch (...) {
    clearResources();
    if (auto eptr = std::current_exception(); eptr) {
      std::rethrow_exception(eptr);
    }
  }
}

SharedMemory::~SharedMemory() { clearResources(); }

Data *SharedMemory::getFreeBuffer() {
  std::lock_guard<NamedMutex> lock(namedMutex_);
  for (size_t i = 0; i < BuffersCount; ++i) {
    if (buffer_->freeIndexes[i]) {
      buffer_->freeIndexes[i] = 0;
      return &buffer_->data[i];
    }
  }
  return nullptr;
}

void SharedMemory::pushToWriteQueue(Data *data) {
  std::lock_guard<NamedMutex> lock(namedMutex_);
  assert(buffer_->writeQueue[BuffersCount - 1] == nullptr);
  memcpy(buffer_->writeQueue + 1, buffer_->writeQueue,
         (BuffersCount - 1) * sizeof(buffer_->writeQueue[0]));
  buffer_->writeQueue[0] = data;
}

Data *SharedMemory::popFromWriteQueue() {
  std::lock_guard<NamedMutex> lock(namedMutex_);
  for (int i = BuffersCount - 1; i >= 0; --i) {
    if (buffer_->writeQueue[i] != nullptr) {
      auto buffer = buffer_->writeQueue[i];
      buffer_->writeQueue[i] = nullptr;
      return buffer;
    }
  }
  return nullptr;
}

void SharedMemory::releaseBuffer(Data *data) {
  std::lock_guard<NamedMutex> lock(namedMutex_);
  for (size_t i = 0; i < BuffersCount; ++i) {
    if (&buffer_->data[i] == data) {
      buffer_->freeIndexes[i] = 1;
      break;
    }
  }
}

void SharedMemory::setReaderDone(bool value) { buffer_->readerDone = value; }

bool SharedMemory::isReaderDone() const { return buffer_->readerDone; }

bool SharedMemory::isBufferFree() const {
  for (size_t i = 0; i < BuffersCount; ++i) {
    if (buffer_->writeQueue[i] != nullptr) {
      return false;
    }
  }
  return true;
}

bool SharedMemory::attemptRelease() { return isReaderDone() && isBufferFree(); }

void SharedMemory::clearResources() { shmctl(id_, IPC_RMID, 0); }