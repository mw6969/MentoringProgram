#include <cstring>
#include <iostream>
#include <sys/shm.h>

#include "SharedMemory.h"

SharedMemory::SharedMemory() {
    // Get shared memory segment of 'Collection' class
    id_ = shmget(696969, sizeof(Collection), IPC_CREAT | 0666);
    if (id_ < 0)
        std::cerr << "Error getting shared memory segment of 'Collection' class\n";

    // Attach shared memory segment of 'Collection' class
    collection_ = (Collection*)shmat(id_, (void*)0, 0);
    if (collection_ == (Collection*)(-1))
        std::cerr << "Error attaching shared memory segment of 'Collection' class\n";
    
    // Get shared memory segment of 'String' class
    id_ = shmget(969696, (sizeof(String)), IPC_CREAT | 0666);
    if (id_ < 0)
        std::cerr << "Error getting shared memory segment of 'String' class\n";

    // Attach shared memory segment of 'String' class
    collection_->strings = (String*)shmat(id_, (void*)0, 0);
    if (collection_->strings == (String*)(-1))
        std::cerr << "Error attaching shared memory segment of 'String' class\n";
    
    collection_->size = 0;
}

void SharedMemory::push(char* data) {
    // Get shared memory segment of 'data' member
    id_ = shmget(6000 + collection_->size, (strlen(data) * sizeof(char)), IPC_CREAT | 0666);
    if (id_ < 0)
        std::cerr << "Error getting shared memory segment of 'data' member\n";

    // Attach shared memory segment of 'data' member
    collection_->strings[collection_->size].data = (char*)shmat(id_, (void*)0, 0);
    if (collection_->strings[collection_->size].data == (char*)(-1))
        std::cerr << "Error attaching shared memory segment of 'data' member\n";

    collection_->strings[collection_->size].identifier = collection_->size;
    strcpy(collection_->strings[collection_->size].data, data);
    collection_->size++;
}

void SharedMemory::pop() {
    if (collection_->size >= 0) {
        // Detach last shared memory segment of 'data' member
        shmdt(collection_->strings[collection_->size - 1].data);
        collection_->size--;
    }
}

char* SharedMemory::front() {
    return collection_->strings[collection_->size - 1].data;
}

void SharedMemory::destroy() {
    // Detach last shared memory segment of 'Collection' class
    shmdt(collection_);

    // Remove identifier
    shmctl(id_, IPC_RMID, 0);
}

bool SharedMemory::empty() {
    return collection_->size == 0;
}

size_t SharedMemory::size() {
    return collection_->size;
}