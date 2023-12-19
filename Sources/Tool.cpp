#include <fstream>
#include <thread>
#include <iostream>

#include "Tool.h"

void Tool::runReader(std::string_view inputFile, std::shared_ptr<SharedMemory> sharedMemory)
{
    std::jthread readerThread(&Tool::reader, this, inputFile, sharedMemory);
}

void Tool::runWriter(std::string_view outputFile, std::shared_ptr<SharedMemory> sharedMemory)
{
    std::jthread writerThread(&Tool::writer, this, outputFile, sharedMemory);
}

void Tool::reader(std::string_view inputFile, std::shared_ptr<SharedMemory> sharedMemory)
{
    std::ifstream inFile{inputFile.data(), std::ios_base::in | std::ios_base::binary};
    if (inFile.is_open())
    {
        std::cout << "Reader has started\n";

        while (inFile)
        {
            std::lock_guard<std::mutex> lockGuard{mutex_};

            auto buffer = sharedMemory.get();
            //inFile.read(sharedMemory.get(), 100);
            //queue_.push(std::make_pair(buffer, inFile.gcount()));

            conditionalVariable_.notify_one();
        }
        inFile.close();

        readerDone_ = true;

        std::cout << "Reader has done\n";
    }
    else
    {
        std::cout << "Error opening file\n";
        abort();
    }
}

void Tool::writer(std::string_view outputFile, std::shared_ptr<SharedMemory> sharedMemory)
{
    std::ofstream outFile{outputFile.data(), std::ios::app | std::ios_base::binary};
    if (outFile.is_open())
    {
        std::cout << "Writer has started\n";

        while (true)
        {
            std::unique_lock<std::mutex> uniqueLock{mutex_};
            conditionalVariable_.wait(uniqueLock, [this]() { return !queue_.empty(); });

            std::string buffer{queue_.front().first};
            size_t bufferSize{queue_.front().second};
            queue_.pop();

            uniqueLock.unlock();

            outFile.write(buffer.data(), bufferSize);

            if (readerDone_ && queue_.empty())
            {
                break;
            }
        }

        outFile.close();

        std::cout << "Writer has done\n";
    }
    else
    {
        std::cout << "Error opening file\n";
        abort();
    }
}
