#include <fstream>
#include <thread>
#include <iostream>

#include "Tool.h"

std::condition_variable Tool::mConditionalVariable;
std::string Tool::mData;
std::mutex Tool::mMutex;

void Tool::copy(const std::string_view sourceName, const std::string_view targetName)
{
    std::jthread readerThread(reader, sourceName);
    std::jthread writerThread(writer, targetName);
}

void Tool::reader(const std::string_view sourceName)
{
    std::cout << "Reader has started\n";

    std::lock_guard<std::mutex> lockGuard{mMutex};

    std::ifstream inFile{sourceName.data()};
    inFile >> mData;
    inFile.close();

    mConditionalVariable.notify_one();

    std::cout << "Reader has done\n";
}

void Tool::writer(const std::string_view targetName)
{
    std::cout << "Writer has started\n";

    std::unique_lock<std::mutex> uniqueLock{mMutex};

    mConditionalVariable.wait(uniqueLock, []() { return !mData.empty(); });

    std::ofstream outFile{targetName.data()};
    outFile << mData;
    outFile.close();

    mData.clear();

    uniqueLock.unlock();

    std::cout << "Writer has done\n";
}