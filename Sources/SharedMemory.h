#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <stddef.h>

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

    void push(char* data);
    void pop();
    String front();
    void destroy();
    bool empty();
    void print();

private:
    Collection* collection_;
    int id_;
};

#endif