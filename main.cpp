#include <string>
#include <iostream>

#include "Tool.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return -1;
    }

    Tool tool;
    tool.copy(argv[1], argv[2]);

    return 0;
}