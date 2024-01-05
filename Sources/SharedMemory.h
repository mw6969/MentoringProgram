#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <memory>
#include <string>
#include <queue>
class SharedMemory
{
public:
    SharedMemory(const std::string& name);
    ~SharedMemory();

    std::queue<std::pair<std::string, size_t>>* get();

private:
    std::queue<std::pair<std::string, size_t>>* queue_;
    std::unique_ptr<std::string> name_;
    int id_;
};

#endif