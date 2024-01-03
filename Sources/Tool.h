#ifndef _TOOL_
#define _TOOL_

#include <string>

class Tool
{
public:
    void reader(std::string_view inputFile, std::string_view memoryName);
    void writer(std::string_view outputFile, std::string_view memoryName);

private:
    bool readerDone_;
};

#endif
