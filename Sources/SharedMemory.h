#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <fcntl.h>
#include <memory>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <queue>
#include <unistd.h>
#include <utility>
class SharedMemory
{
public:
    SharedMemory(const std::string& name);
    ~SharedMemory();

    bool empty();
    std::string itemBuf();
    std::size_t itemSize();
    void pop();
    void push(const std::string& data, const size_t size);
    std::shared_ptr<pthread_cond_t> getConditionVariable() const;
    std::shared_ptr<pthread_mutex_t> getMutex() const;

private:
    void open();
    void memoryMap(const std::string &data, const size_t size);
    void fileTruncate();

private:
    std::queue<std::pair<std::string, size_t>> queue_;
    std::unique_ptr<std::string> name_;
    int fileDescriptor_;
    std::shared_ptr<pthread_cond_t> conditionVariable_;
    std::shared_ptr<pthread_mutex_t> mutex_;
};

#endif