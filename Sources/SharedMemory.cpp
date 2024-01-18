#include <cstring>
#include <iostream>
#include <sys/shm.h>

#include "SharedMemory.h"

SharedMemory::SharedMemory() {
  if (collectionId_ = shmget(9669699, sizeof(Collection), IPC_CREAT | 0666);
      collectionId_ < 0) {
    std::cerr << "Error getting shared memory segment of 'Collection' class\n";
    return;
  }

  if (collection_ = (Collection *)shmat(collectionId_, (void *)0, 0);
      collection_ == (Collection *)(-1)) {
    std::cerr
        << "Error attaching shared memory segment of 'Collection' class\n";
    return;
  }

  if (stringId_ = shmget(9999669, (100 * sizeof(String)), IPC_CREAT | 0666);
      stringId_ < 0) {
    std::cerr << "Error getting shared memory segment of 'String' class\n";
    return;
  }

  if (collection_->strings = (String *)shmat(stringId_, (void *)0, 0);
      collection_->strings == (String *)(-1)) {
    std::cerr << "Error attaching shared memory segment of 'String' class\n";
    return;
  }

  collection_->first = 0;
  collection_->size = 0;
}

void SharedMemory::push(const char *data) {
  collection_->strings[collection_->size].id = collection_->size;
  strcpy(collection_->strings[collection_->size].data, data);

  collection_->size++;
}

void SharedMemory::popFront() { collection_->first++; }

const char *SharedMemory::front() {
  if (collection_->first < collection_->size)
    return collection_->strings[collection_->first].data;

  return nullptr;
}

void SharedMemory::destroy() {
  shmdt(collection_->strings);
  shmctl(stringId_, IPC_RMID, 0);

  shmdt(collection_);
  shmctl(collectionId_, IPC_RMID, 0);
}

bool SharedMemory::empty() { return size() == 0; }

size_t SharedMemory::size() { return collection_->size - collection_->first; }