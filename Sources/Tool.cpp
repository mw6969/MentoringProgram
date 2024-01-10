
#include <cstring>
#include <fstream>
#include <iostream>
 
#include "Tool.h"

void Tool::reader(const std::string& fileName) {
    std::ifstream inFile{fileName.data(), std::ios_base::in | std::ios_base::binary};
    if (inFile.is_open()) {
        std::cout << "Reader has started\n";

        while (inFile) {
            pthread_mutex_lock(mutex_.get());

            size_t bufferSize{100};
            char* buffer{new char[bufferSize]};
            inFile.read(buffer, bufferSize);
            sharedMemory_.push(buffer);

            pthread_mutex_unlock(mutex_.get());
            pthread_cond_signal(conditionVariable_.get());
        }
        inFile.close();

        readerDone_ = true;

        std::cout << "Reader has done\n";
    } else {
        std::cout << "Error opening input file\n";
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
            sharedMemory_.pop();

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
