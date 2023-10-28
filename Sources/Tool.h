#ifndef _TOOL_
#define _TOOL_

#include <condition_variable>
#include <mutex>
#include <string_view>
#include <queue>

class Tool
{
public:
    static void copy(std::string_view inputFile, std::string_view outputFile);

private:
    static std::condition_variable mConditionalVariable;
    static std::mutex mMutex;
    static bool mReaderDone;
    static std::queue<char*> mQueue;

private:
    static void reader(std::string_view inputFile);
    static void writer(std::string_view outputFile);
};

#endif