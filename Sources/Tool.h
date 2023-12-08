#ifndef _TOOL_
#define _TOOL_

#include <condition_variable>
#include <mutex>
#include <string>
#include <queue>
#include <utility>

class Tool
{
public:
    void copy(std::string_view inputFile, std::string_view outputFile);

private:
    std::condition_variable conditionalVariable_;
    std::mutex mutex_;
    bool readerDone_;
    std::queue<std::pair<std::string, size_t>> queue_;

private:
    void reader(std::string_view inputFile);
    void writer(std::string_view outputFile);
};

#endif
