
#include <cstring>
#include <fstream>
#include <iostream>

#include "SharedSemaphore.h"
#include "Tool.h"

void Tool::reader(const std::string &fileName) {
  std::ifstream inFile{fileName.data(),
                       std::ios_base::in | std::ios_base::binary};
  if (inFile.is_open()) {
    std::cout << "Reader has started\n";

    while (inFile) {
      short index;
      {
        SharedSemaphore sharedSemaphore;
        index = sharedMemory_.getFreeBufferIndex();
      }

      if (index > -1) {
        char buffer[100] = {0};
        inFile.read(buffer, 99);
        sharedMemory_.setData(index, buffer);
      }

      {
        SharedSemaphore sharedSemaphore;
        sharedMemory_.setReadyForWriteBufferIndex(index);
      }
    }
    inFile.close();

    readerDone_ = true;

    std::cout << "Reader has done\n";
  } else {
    std::cout << "Error opening input file\n";
  }
}

void Tool::writer(const std::string &fileName) {
  std::ofstream outFile{fileName.data(), std::ios_base::binary};
  if (outFile.is_open()) {
    std::cout << "Writer has started\n";

    while (true) {
      std::string data;
      short index;
      {
        SharedSemaphore sharedSemaphore;
        index = sharedMemory_.getFirstReadyForWriteBufferIndex();

        if (index > -1) {
          sharedMemory_.shiftFirstReadyForWriteBufferIndex();
          data = sharedMemory_.getData(index);
        }
      }

      if (!data.empty()) {
        outFile.write(data.c_str(), data.length());
      }

      {
        SharedSemaphore sharedSemaphore;
        sharedMemory_.setFirstFreeBufferIndex(index);
      }

      if (readerDone_ && sharedMemory_.checkFreeBufferIndex()) {
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