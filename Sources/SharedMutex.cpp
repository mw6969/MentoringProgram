#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "SharedMutex.h"

#define MYMUTEX "mymutex"

SharedMutex::SharedMutex() {
    // Create and open a new object, or open an existing object
    int id_{shm_open(MYMUTEX, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG)};
    if (id_ < 0)
        std::cerr << "shm_open failed with " << MYMUTEX << "\n";

    // Set the size of the shared memory object
    if (ftruncate(id_, sizeof(pthread_mutex_t)) == -1)
        std::cerr << "ftruncate failed with " << MYMUTEX << "\n";

    // Map the shared memory object into the virtual address space of the calling process
    mutex_ = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, id_, 0);
    if (mutex_ == MAP_FAILED)
        std::cerr << "mmap failed with " << MYMUTEX << "\n";

    // Set mutex shared between processes
    pthread_mutexattr_init(&mutexattr_);
    pthread_mutexattr_setpshared(&mutexattr_, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex_, &mutexattr_);
}

SharedMutex::~SharedMutex() {
    // Destroy mutex
    pthread_mutex_destroy(mutex_);

    // Destroy mutex attribute
    pthread_mutexattr_destroy(&mutexattr_);

    // Release the shared memory object
    shm_unlink(MYMUTEX);

    // Unmap the shared memory object
    munmap(mutex_, sizeof(pthread_mutex_t));
}

pthread_mutex_t* SharedMutex::get() {
    return mutex_;
}