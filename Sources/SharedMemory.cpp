#include <cstring>
#include <iostream>
#include <sys/shm.h>

#include "SharedMemory.h"

constexpr int SharedMemoryKey = 999;

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

  for (size_t i = 0; i < BuffersCount; i++) {
    buffer_->freeIndexes[i] = i;
    buffer_->readyForWriteIndexes[i] = -1;
  }

  buffer_->readerDone = false;
}

void SharedMemory::setData(const short index, const std::string &data) {
  strcpy(buffer_->data[index], data.data());
}

std::string SharedMemory::getData(const short index) const {
  return buffer_->data[index];
}

short SharedMemory::getFreeBufferIndex() {
  for (size_t i = 0; i < BuffersCount; i++) {
    if (buffer_->freeIndexes[i] > -1) {
      short index = buffer_->freeIndexes[i];
      setReadyForWriteBufferIndex(index);
      buffer_->freeIndexes[i] = -1;
      return index;
    }
  }
  return -1;
}

bool SharedMemory::isBufferFree() const {
  for (size_t i = 0; i < BuffersCount; i++) {
    if (buffer_->readyForWriteIndexes[i] != -1) {
      return false;
    }
  }
  return true;
}

short SharedMemory::getReadyForWriteBufferIndex() {
  short index = -1;
  for (size_t i = BuffersCount - 1; i >= 0; i--) {
    if (i == BuffersCount - 1) {
      index = buffer_->readyForWriteIndexes[i];
      setFirstFreeBufferIndex(index);
    }
    if (i != 0) {
      buffer_->readyForWriteIndexes[i] = buffer_->readyForWriteIndexes[i - 1];
    } else {
      buffer_->readyForWriteIndexes[i] = -1;
      break;
    }
  }
  return index;
}

void SharedMemory::setReaderDone(bool value) { buffer_->readerDone = value; }

bool SharedMemory::isReaderDone() const { return buffer_->readerDone; }

void SharedMemory::destroy() {
  shmdt(buffer_);
  shmctl(id_, IPC_RMID, 0);
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
  for (size_t i = BuffersCount - 1; i >= 0; i--) {
    if (i != 0) {
      buffer_->readyForWriteIndexes[i] = buffer_->readyForWriteIndexes[i - 1];
    } else {
      buffer_->readyForWriteIndexes[i] = index;
      break;
    }
  }
}