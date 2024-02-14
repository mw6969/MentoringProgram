#include <cstring>
#include <iostream>
#include <sys/shm.h>

#include "SharedMemory.h"

constexpr int SharedMemoryKey = 999;

SharedMemory::SharedMemory() {
  if (id_ = shmget(SharedMemoryKey, sizeof(Buffer), IPC_CREAT | 0666); id_ < 0) {
    std::cerr << "Error getting shared memory segment of 'Collection' class\n";
    return;
  }

  if (buffer_ = (Buffer *)shmat(id_, nullptr, 0); buffer_ == (Buffer *)(-1)) {
    std::cerr << "Error attaching shared memory segment of 'Collection' class\n";
    return;
  }

  for (size_t i = 0; i < BuffersCount; i++) {
    buffer_->freeIndexes[i] = i;
    buffer_->readyForWriteIndexes[i] = -1;
  }
}

void SharedMemory::setData(const short index, const std::string &data) {
  strcpy(buffer_->data[index], data.data());
}

std::string SharedMemory::getData(const short index) const {
  return buffer_->data[index];
}

short SharedMemory::getFreeBufferIndex() const {
  for (size_t i = 0; i < BuffersCount; i++) {
    if (buffer_->freeIndexes[i] > -1) {
      int index = buffer_->freeIndexes[i];
      buffer_->freeIndexes[i] = -1;
      return index;
    }
  }

  return -1;
}

void SharedMemory::setFirstFreeBufferIndex(const short index) {
  for (size_t i = 0; i < BuffersCount; i++) {
    if (buffer_->freeIndexes[i] == -1) {
      buffer_->freeIndexes[i] = index;
      break;
    }
  }
}

void SharedMemory::setReadyForWriteBufferIndex(const short index) {
  buffer_->readyForWriteIndexes[index] = index;
}

bool SharedMemory::checkFreeBufferIndex() {
  for (size_t i = 0; i < BuffersCount; i++) {
    if (buffer_->freeIndexes[i] == -1) {
      return false;
    }
  }

  return true;
}

void SharedMemory::destroy() {
  shmdt(buffer_);
  shmctl(id_, IPC_RMID, 0);
}

void SharedMemory::shiftFirstReadyForWriteBufferIndex() {
  for (size_t i = 0; i < BuffersCount; i++) {
    if (buffer_->readyForWriteIndexes[i] != -1) {
      buffer_->readyForWriteIndexes[i] = -1;
      break;
    }
  }
}

short SharedMemory::getFirstReadyForWriteBufferIndex() const {
  for (size_t i = 0; i < BuffersCount; i++) {
    if (buffer_->readyForWriteIndexes[i] > -1) {
      return buffer_->readyForWriteIndexes[i];
    }
  }
  return -1;
}