#include <assert.h>
#include <cstring>
#include <iostream>
#include <mutex>
#include <sys/shm.h>

#include "SharedMemory.h"

constexpr int SharedMemoryKey = 1104;

SharedMemory::SharedMemory() {
  if (id_ = shmget(SharedMemoryKey, sizeof(Buffer), IPC_CREAT | 0666);
      id_ < 0) {
    std::cerr << "Error getting shared memory segment of 'Collection' class\n";
    return;
  }

  if (buffer_ = (Buffer *)shmat(id_, nullptr, 0); buffer_ == (Buffer *)(-1)) {
    std::cerr
        << "Error attaching shared memory segment of 'Collection' class\n";
    return;
  }

  for (size_t i = 0; i < BuffersCount; ++i) {
    buffer_->freeIndexes[i] = 1;
    buffer_->writeQueue[i] = nullptr;
  }
  buffer_->readerDone = false;
}

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

bool SharedMemory::attemptRelease() {
  if (isReaderDone() && isBufferFree()) {
    shm_unlink(NamedMutex::getName());
    shmdt(buffer_);
    shmctl(id_, IPC_RMID, 0);
    return true;
  }
  return false;
}