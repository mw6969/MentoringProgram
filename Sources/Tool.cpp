#include <fstream>
#include <iostream>
 
#include "SharedMemory.h"
#include "SharedConditionVariable.h"
#include "SharedMutex.h"
#include "Tool.h"

void Tool::reader(const std::string& fileName) {
    std::ifstream inFile{fileName.data(), std::ios_base::in | std::ios_base::binary};
    if (inFile.is_open()) {
        std::cout << "Reader has started\n";

        SharedMemory sharedMemory;
        SharedMutex mutex;
        SharedConditionVariable conditionVariable;

        while (inFile) {
            pthread_mutex_lock(mutex.get());

            size_t bufferSize{100};
            char* buffer{new char[bufferSize]};
            inFile.read(buffer, bufferSize);
            sharedMemory.push(buffer);

            pthread_cond_signal(conditionVariable.get());
            pthread_mutex_unlock(mutex.get());
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

        SharedMemory sharedMemory;
        SharedMutex mutex;
        SharedConditionVariable conditionVariable;

        while (true) {
            pthread_mutex_lock(mutex.get());

            if (sharedMemory.empty()) {
                pthread_cond_wait(conditionVariable.get(), mutex.get());
            }

            std::string buffer{sharedMemory.front().data};
            sharedMemory.pop();

            pthread_mutex_unlock(mutex.get());

            outFile.write(buffer.data(), 100);

            if (readerDone_ && sharedMemory.empty()) {
                sharedMemory.destroy();
                break;
            }
        }

        outFile.close();

        std::cout << "Writer has done\n";
    } else {
        std::cout << "Error opening output file\n";
    }
}
