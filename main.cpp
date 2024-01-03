#include <iostream>

#include "SharedMemory.h"
#include "Tool.h"

struct shmbuf {
    std::mutex mutex;
    size_t size;
    char buf[1024];
};

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "Invalid input arguments, use: <source> <destination> <name of shared memory object>\n";
        return -1;
    }

    shm_unlink(argv[3]);
    SharedMemory sharedMemory(argv[3]);
    auto mutex{sharedMemory.getMutex().get()};
    auto conditionVariable{sharedMemory.getConditionVariable().get()};
    Tool tool;

    // Set mutex shared between processes
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);

    // Set condition shared between processes
    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(conditionVariable, &cattr);
    pthread_condattr_destroy(&cattr);

    if (pid_t pid{fork()}; pid == 0)
    {
        tool.reader(argv[1], argv[3]);
    }
    else
    {
        tool.writer(argv[2], argv[3]);
    }

    return 0;
}