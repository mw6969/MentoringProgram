#ifndef _TOOL_
#define _TOOL_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <utility>

#include "SharedMemory.h"

class Tool
{
public:
    void runReader(std::string_view inputFile, std::shared_ptr<SharedMemory> sharedMemory);
    void runWriter(std::string_view outputFile, std::shared_ptr<SharedMemory> sharedMemory);

private:
    std::condition_variable conditionalVariable_;
    std::mutex mutex_;
    bool readerDone_;
    std::queue<std::pair<std::string, size_t>> queue_;

private:
    void reader(std::string_view inputFile, std::shared_ptr<SharedMemory> sharedMemory);
    void writer(std::string_view outputFile, std::shared_ptr<SharedMemory> sharedMemory);
};

#endif
