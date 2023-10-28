#include <fstream>
#include <thread>
#include <iostream>

#include "Tool.h"

constexpr size_t BUFFER_SIZE = 100;

std::condition_variable Tool::mConditionalVariable;
std::mutex Tool::mMutex;
bool Tool::mReaderDone;
std::queue<char*> Tool::mQueue;

void Tool::copy(std::string_view inputFile, std::string_view outputFile)
{
    std::jthread readerThread(reader, inputFile);
    std::jthread writerThread(writer, outputFile);
}

void Tool::reader(std::string_view inputFile)
{
    std::ifstream inFile{inputFile.data()};
    if (inFile.is_open())
    {
        std::cout << "Reader has started\n";

        while (inFile)
        {
            std::lock_guard<std::mutex> lockGuard{mMutex};

            char* buffer{new char[BUFFER_SIZE]};
            inFile.read(buffer, BUFFER_SIZE);
            mQueue.push(buffer);
            
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
    std::ofstream outFile{outputFile.data(), std::ios::app};
    if (outFile.is_open())
    {
        std::cout << "Writer has started\n";

        while (true)
        {
            std::unique_lock<std::mutex> uniqueLock{mMutex};
            mConditionalVariable.wait(uniqueLock, []() { return !mQueue.empty(); });

            char* buffer{mQueue.front()};
            outFile.write(buffer, BUFFER_SIZE);
            delete[] buffer;

            mQueue.pop();

            uniqueLock.unlock();

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