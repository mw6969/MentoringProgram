#include <iostream>
#include <unistd.h>

#include "Tool.h"
 
int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << "Format: <source> <destination> <shared memory name>\n";
        return -1;
    }

    Tool tool;
    if (pid_t pid{fork()}; pid < 0) {
        std::cerr << "fork failure\n";
        return -1;
    } else if (pid == 0) {
        tool.reader(argv[1], argv[3]);
    } else {
        tool.writer(argv[2], argv[3]);
    }
    return 0;
}