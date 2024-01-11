
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
            pthread_mutex_lock(mutex_.get());

            std::string line;
            std::getline(inFile, line);
            sharedMemory_.push(line.data());

            pthread_mutex_unlock(mutex_.get());
            pthread_cond_signal(conditionVariable_.get());
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
            pthread_mutex_lock(mutex_.get());
            if (sharedMemory_.empty())
                pthread_cond_wait(conditionVariable_.get(), mutex_.get());

            char* data{sharedMemory_.front()};
            char* buffer{new char[strlen(data)]};
            strcpy(buffer, data);
            sharedMemory_.popFront();

            pthread_mutex_unlock(mutex_.get());
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
