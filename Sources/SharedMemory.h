#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <stddef.h>
#include <string>
struct String {
    size_t identifier;
    char* data;
};

struct Collection {
    size_t size;
    String* strings;
};
class SharedMemory
{
public:
    SharedMemory();
    ~SharedMemory() = default;

    void push(const std::string& data);
    void pop();
    String front();
    void destroy();
    bool empty();
    size_t size();

private:
    Collection* collection_;
    int id_;
};

#endif