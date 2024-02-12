#include <cstring>
#include <iostream>
#include <sys/shm.h>

#include "SharedMemory.h"

SharedMemory::SharedMemory() {
  if (collectionId_ = shmget(66, sizeof(Collection), IPC_CREAT | 0666);
      collectionId_ < 0) {
    std::cerr << "Error getting shared memory segment of 'Collection' class\n";
    return;
  }

  if (collection_ = (Collection *)shmat(collectionId_, nullptr, 0);
      collection_ == (Collection *)(-1)) {
    std::cerr
        << "Error attaching shared memory segment of 'Collection' class\n";
    return;
  }

  if (stringId_ = shmget(99, (100 * sizeof(String)), IPC_CREAT | 0666);
      stringId_ < 0) {
    std::cerr << "Error getting shared memory segment of 'String' class\n";
    return;
  }

  if (collection_->strings = (String *)shmat(stringId_, nullptr, 0);
      collection_->strings == (String *)(-1)) {
    std::cerr << "Error attaching shared memory segment of 'String' class\n";
    return;
  }

  collection_->first = 0;
  collection_->size = 0;
}

void SharedMemory::push(const std::string &data) {
  collection_->strings[collection_->size].id = collection_->size;
  strcpy(collection_->strings[collection_->size].data, data.data());

  collection_->size++;
}

void SharedMemory::popFront() { collection_->first++; }

std::string SharedMemory::front() {
  if (collection_->first < collection_->size) {
    std::string data{collection_->strings[collection_->first].data};
    return data;
  }

  return "";
}

void SharedMemory::destroy() {
  shmdt(collection_->strings);
  shmctl(stringId_, IPC_RMID, 0);

  shmdt(collection_);
  shmctl(collectionId_, IPC_RMID, 0);
}

bool SharedMemory::empty() {
  return collection_->size - collection_->first == 0;
}