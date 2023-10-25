#ifndef _TOOL_
#define _TOOL_

#include <condition_variable>
#include <mutex>
#include <string>

class Tool
{
public:
    static void copy(const std::string_view sourceName, const std::string_view targetName);

private:
    static std::condition_variable mConditionalVariable;
    static std::string mData;
    static std::mutex mMutex;

private:
    static void reader(const std::string_view sourceName);
    static void writer(const std::string_view targetName);
};

#endif