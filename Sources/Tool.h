#ifndef _TOOL_
#define _TOOL_

#include <condition_variable>
#include <mutex>
#include <string_view>
#include <queue>
#include <utility>

class Tool
{
public:
    void copy(std::string_view inputFile, std::string_view outputFile);

private:
    std::condition_variable mConditionalVariable;
    std::mutex mMutex;
    bool mReaderDone;
    std::queue<std::pair<char*, size_t>> mQueue;

private:
    void reader(std::string_view inputFile);
    void writer(std::string_view outputFile);
};

#endif
