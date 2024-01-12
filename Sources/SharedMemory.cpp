#include <cstring>
#include <iostream>
#include <sys/shm.h>

#include "SharedMemory.h"

SharedMemory::SharedMemory() {
    // Get shared memory segment of 'Collection' class
    collectionId_ = shmget(123, sizeof(Collection), IPC_CREAT | 0666);
    if (collectionId_ < 0) {
        std::cerr << "Error getting shared memory segment of 'Collection' class\n";
        return;
    }

    // Attach shared memory segment of 'Collection' class
    collection_ = (Collection*)shmat(collectionId_, NULL, 0);
    if (collection_ < (Collection*)NULL) {
        std::cerr << "Error attaching shared memory segment of 'Collection' class\n";
        return;
    }
    
    // Get shared memory segment of 'String' class
    stringId_ = shmget(1234, (sizeof(String)), IPC_CREAT | 0666);
    if (stringId_ < 0) {
        std::cerr << "Error getting shared memory segment of 'String' class\n";
        return;
    }

    // Attach shared memory segment of 'String' class
    collection_->strings = (String*)shmat(stringId_, NULL, 0);
    if (collection_->strings < (String*)NULL) {
        std::cerr << "Error attaching shared memory segment of 'String' class\n";
        return;
    }
    
    collection_->first = 0;
    collection_->size = 0;
}

void SharedMemory::push(char* data) {
    // Get shared memory segment of 'data' member
    int dataId = shmget(90 + collection_->size, (strlen(data) * sizeof(char)), IPC_CREAT | 0666);
    if (dataId < 0) {
        std::cerr << "Error getting shared memory segment of 'data' member\n";
        return;
    }

    // Attach shared memory segment of 'data' member
    collection_->strings[collection_->size].data = (char*)shmat(dataId, NULL, 0);
    if (collection_->strings[collection_->size].data < (char*)NULL) {
        std::cerr << "Error attaching shared memory segment of 'data' member\n";
        return;
    }

    collection_->strings[collection_->size].id = dataId;
    strcpy(collection_->strings[collection_->size].data, data);
    collection_->size++;
}

void SharedMemory::popFront() {
    shmdt(collection_->strings[0].data);
    shmctl(collection_->strings[0].id, IPC_RMID, 0);

    collection_->first++;
}

char* SharedMemory::front() {
    if (collection_->first < collection_->size)
    std::cout << collection_->strings[collection_->first].data << "\n";
        return collection_->strings[collection_->first].data;
    
    return nullptr;
}

void SharedMemory::destroy() {
    shmdt(collection_->strings);
    shmctl(stringId_, IPC_RMID, 0);

    shmdt(collection_);
    shmctl(collectionId_, IPC_RMID, 0);
}

bool SharedMemory::empty() {
    return size() == 0;
}

size_t SharedMemory::size() {
    return collection_->size - collection_->first;
}