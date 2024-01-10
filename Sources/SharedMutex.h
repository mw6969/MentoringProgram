#ifndef _SHAREDMUTEX_
#define _SHAREDMUTEX_

#include <string>

class SharedMutex
{
public:
    SharedMutex();
    ~SharedMutex();

    pthread_mutex_t* get();

private:
    pthread_mutexattr_t mutexattr_;
    pthread_mutex_t* mutex_;
    int id_;
};

#endif