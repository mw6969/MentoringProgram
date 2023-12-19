#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <memory>
#include <string>

class SharedMemory
{
public:
    SharedMemory() = default;
    ~SharedMemory();

    std::shared_ptr<SharedMemory> create();

private:
    void* data_;
};

#endif