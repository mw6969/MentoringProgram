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
    tool.runReader(argv[1], memory);
    tool.runWriter(argv[2], memory);

    return 0;
}