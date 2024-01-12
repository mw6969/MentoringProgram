
#include <cstring>
#include <fstream>
#include <iostream>
 
#include "Tool.h"

void Tool::reader(const std::string& fileName) {
    std::cout << "Reader has started\n";

    std::ifstream inFile;
    inFile.open(fileName.data(), std::ios_base::in | std::ios_base::binary);
    if (inFile.fail()) {
        std::cout << "Error opening input file\n";
    } else {
        while (inFile.good()) {
            ss.acquire();

            std::string line;
            std::getline(inFile, line);
            sharedMemory_.push(line.data());

            ss.release();
        }
        inFile.close();

        readerDone_ = true;
        std::cout << "Reader has done\n";
    }
}

void Tool::writer(const std::string& fileName) {
    std::ofstream outFile{fileName.data(), std::ios::app | std::ios_base::binary};
    if (outFile.is_open()) {
        std::cout << "Writer has started\n";

        while (true) {
            ss.acquire();

            char* buffer{nullptr};
            if (char* data{sharedMemory_.front()}; data != nullptr) {
                buffer = new char[strlen(data)];
                strcpy(buffer, data);
                sharedMemory_.popFront();
            }

            ss.release();

            if (buffer)
                outFile.write(buffer, strlen(buffer));

            if (readerDone_ && sharedMemory_.empty()) {
                sharedMemory_.destroy();
                break;
            }
        }

        outFile.close();

        std::cout << "Writer has done\n";
    } else {
        std::cout << "Error opening output file\n";
    }
}
