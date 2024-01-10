#include <iostream>
#include <unistd.h>

#include "Tool.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Format: <source> <destination>\n";
        return -1;
    }

    Tool tool;
    if (pid_t c_pid{fork()}; c_pid == -1) {
        std::cout << "Fork error\n";
        return -1;
    } 
    else if (c_pid > 0) {
        tool.reader(argv[1]);
    } 
    else {
        tool.writer(argv[2]);
    }

    return 0;
}