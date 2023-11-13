#include <string>
#include <iostream>

#include "Tool.h"

int main(int argc, char *argv[])
{
    std::string inputFile;
    std::cout << "Input file path:";
    std::getline(std::cin, inputFile);

    std::string outputFile;
    std::cout << "Output file path:";
    std::getline(std::cin, outputFile);

    Tool tool;
    tool.copy(inputFile, outputFile);

    return 0;
}