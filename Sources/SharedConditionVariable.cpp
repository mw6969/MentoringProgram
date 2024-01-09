#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "SharedConditionVariable.h"

#define MYCOND "mycond"

SharedConditionVariable::SharedConditionVariable() {
    // Create and open a new object, or open an existing object
    int id_{shm_open(MYCOND, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG)};
    if (id_ < 0)
        std::cerr << "shm_open failed with " << MYCOND << "\n";

    // Set the size of the shared memory object
    if (ftruncate(id_, sizeof(pthread_cond_t)) == -1)
        std::cerr << "ftruncate failed with " << MYCOND << "\n";

    // Map the shared memory object into the virtual address space of the calling process
    conditionVariable_ = (pthread_cond_t *) mmap(NULL, sizeof(pthread_cond_t), PROT_READ | PROT_WRITE, MAP_SHARED, id_, 0);
    if (conditionVariable_ == MAP_FAILED)
        std::cerr << "mmap failed with " << MYCOND << "\n";

    // Set mutex shared between processes
    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(conditionVariable_, &cattr);
    pthread_condattr_destroy(&cattr);
}

SharedConditionVariable::~SharedConditionVariable() {
    // Destroy mutex
    pthread_cond_destroy(conditionVariable_);

    // Release the shared memory object
    shm_unlink(MYCOND);
}

pthread_cond_t* SharedConditionVariable::get() {
    return conditionVariable_;
}