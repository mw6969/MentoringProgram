#ifndef _TOOL_
#define _TOOL_

#include <string>

class Tool
{
public:
    void reader(const std::string& fileName, const std::string& memoryName);
    void writer(const std::string& fileName, const std::string& memoryName);

private:
    bool readerDone_;
};

#endif
