#include <iostream>
#include <string.h>
#include <unistd.h>

#include "SharedMemory.h"
#include "Tool.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return -1;
    }

    SharedMemory sharedMemory;
    auto memory{sharedMemory.create()};
    
    Tool tool;

    if (pid_t pid{fork()}; pid == 0)
    {
        tool.runReader(argv[1], memory);
    }
    else
    {
        tool.runWriter(argv[2], memory);
    }

    return 0;
}