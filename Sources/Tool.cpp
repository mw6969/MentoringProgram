#include <fstream>
#include <thread>
#include <iostream>

#include "SharedMemory.h"
#include "Tool.h"

void Tool::reader(std::string_view inputFile, std::string_view memoryName)
{
    std::ifstream inFile{inputFile.data(), std::ios_base::in | std::ios_base::binary};
    if (inFile.is_open())
    {
        std::cout << "Reader has started\n";

        while (inFile)
        {
            SharedMemory sharedMemory(memoryName.data());
            auto mutex{sharedMemory.getMutex().get()};
            auto conditionVariable{sharedMemory.getConditionVariable().get()};
            pthread_mutex_lock(mutex);

            std::string buffer;
            inFile.read(buffer.data(), 100);
            sharedMemory.push(buffer, 100);
            
            pthread_cond_signal(conditionVariable);
            pthread_mutex_unlock(mutex);
        }
        inFile.close();

        readerDone_ = true;

        std::cout << "Reader has done\n";
    }
    else
    {
        std::cout << "Error opening input file\n";
    }
}

void Tool::writer(std::string_view outputFile, std::string_view memoryName)
{
    std::ofstream outFile{outputFile.data(), std::ios::app | std::ios_base::binary};
    if (outFile.is_open())
    {
        std::cout << "Writer has started\n";

        while (true)
        {
            SharedMemory sharedMemory(memoryName.data());
            auto mutex{sharedMemory.getMutex().get()};
            auto conditionVariable{sharedMemory.getConditionVariable().get()};
            pthread_mutex_lock(mutex);
            pthread_cond_wait(conditionVariable, mutex);

            std::string buffer{sharedMemory.itemBuf()};
            size_t bufferSize{sharedMemory.itemSize()};
            sharedMemory.pop();

            pthread_mutex_unlock(mutex);

            outFile.write(buffer.data(), bufferSize);

            if (readerDone_ && sharedMemory.empty())
            {
                break;
            }
        }

        outFile.close();

        std::cout << "Writer has done\n";
    }
    else
    {
        std::cout << "Error opening output file\n";
    }
}
