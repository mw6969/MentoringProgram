#include <fstream>
#include <iostream>

#include "SharedMemory.h"
#include "SharedConditionVariable.h"
#include "SharedMutex.h"
#include "Tool.h"

void Tool::reader(const std::string& fileName, const std::string& memoryName)
{
    std::ifstream inFile{fileName.data(), std::ios_base::in | std::ios_base::binary};
    if (inFile.is_open()) {
        std::cout << "Reader has started\n";

        while (inFile) {
            SharedMemory sharedMemory(memoryName.data());
            auto queue{sharedMemory.get()};
            SharedMutex mutex;
            SharedConditionVariable conditionVariable;

            pthread_mutex_lock(mutex.get());

            std::string buffer;
            inFile.read(buffer.data(), 100);
            queue->emplace(std::make_pair(buffer, 100));

            pthread_cond_signal(conditionVariable.get());
            pthread_mutex_unlock(mutex.get());
        }
        inFile.close();

        readerDone_ = true;

        std::cout << "Reader has done\n";
        exit(0);
    } else {
        std::cout << "Error opening input file\n";
    }
}

void Tool::writer(const std::string& fileName, const std::string& memoryName)
{
    std::ofstream outFile{fileName.data(), std::ios::app | std::ios_base::binary};
    if (outFile.is_open()) {
        std::cout << "Writer has started\n";

        while (true) {
            SharedMemory sharedMemory(memoryName.data());
            auto queue{sharedMemory.get()};
            SharedMutex mutex;
            SharedConditionVariable conditionVariable;

            pthread_mutex_lock(mutex.get());
            pthread_cond_wait(conditionVariable.get(), mutex.get());

            std::string buffer{queue->front().first};
            size_t size{queue->front().second};
            queue->pop();

            pthread_mutex_unlock(mutex.get());

            outFile.write(buffer.data(), size);

            if (readerDone_ && queue->empty()) {
                break;
            }
        }

        outFile.close();

        std::cout << "Writer has done\n";
        exit(0);
    } else {
        std::cout << "Error opening output file\n";
    }
}
