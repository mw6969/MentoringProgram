#include <fstream>
#include <thread>
#include <iostream>

#include "Tool.h"

void Tool::copy(std::string_view inputFile, std::string_view outputFile)
{
    std::jthread readerThread(&Tool::reader, this, inputFile);
    std::jthread writerThread(&Tool::writer, this, outputFile);
}

void Tool::reader(std::string_view inputFile)
{
    std::ifstream inFile{inputFile.data(), std::ios_base::in | std::ios_base::binary};
    if (inFile.is_open())
    {
        std::cout << "Reader has started\n";

        while (inFile)
        {
            std::lock_guard<std::mutex> lockGuard{mMutex};

            size_t bufferSize{100};
            char* buffer{new char[bufferSize]};
            inFile.read(buffer, bufferSize);
            mQueue.push(std::make_pair(buffer, inFile.gcount()));
            
            mConditionalVariable.notify_one();
        }
        inFile.close();

        mReaderDone = true;

        std::cout << "Reader has done\n";
    }
    else
    {
        std::cout << "Error openning file\n";
        abort();
    }
}

void Tool::writer(std::string_view outputFile)
{
    std::ofstream outFile{outputFile.data(), std::ios::app | std::ios_base::binary};
    if (outFile.is_open())
    {
        std::cout << "Writer has started\n";

        while (true)
        {
            std::unique_lock<std::mutex> uniqueLock{mMutex};
            mConditionalVariable.wait(uniqueLock, [this]() { return !mQueue.empty(); });

            char* buffer{mQueue.front().first};
            size_t bufferSize{mQueue.front().second};
            mQueue.pop();

            uniqueLock.unlock();

            outFile.write(buffer, bufferSize);
            delete[] buffer;

            if (mReaderDone && mQueue.empty())
            {
                break;
            }
        }

        outFile.close();

        std::cout << "Writer has done\n";
    }
    else
    {
        std::cout << "Error openning file\n";
        abort();
    }
}
