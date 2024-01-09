#ifndef _TOOL_
#define _TOOL_

#include <string>

class Tool
{
public:
    Tool() = default;
    ~Tool() = default;

    void reader(const std::string& fileName);
    void writer(const std::string& fileName);

private:
    bool readerDone_;
};

#endif
