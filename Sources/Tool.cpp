
#include <fstream>
#include <iostream>

#include "SharedSemaphore.h"
#include "Tool.h"

void Tool::reader(const std::string &fileName) {
  if (std::ifstream inFile{fileName.data(),
                           std::ios_base::in | std::ios_base::binary};
      inFile.is_open()) {
    std::cout << "Reader has started\n";

    while (inFile) {
      SharedSemaphore sharedSemaphore;
      if (short index = sharedMemory_.getFreeBufferIndex(); index > -1) {
        char buffer[100] = {0};
        inFile.read(buffer, 99);
        sharedMemory_.setData(index, buffer);
      }
    }
    inFile.close();

    sharedMemory_.setReaderDone(true);

    std::cout << "Reader has done\n";
  } else {
    std::cout << "Error opening input file\n";
  }
}

void Tool::writer(const std::string &fileName) {
  if (std::ofstream outFile{fileName.data(), std::ios_base::binary};
      outFile.is_open()) {
    std::cout << "Writer has started\n";

    while (true) {
      {
        SharedSemaphore sharedSemaphore;
        if (short index = sharedMemory_.getReadyForWriteBufferIndex();
            index > -1) {
          if (std::string data = sharedMemory_.getData(index); !data.empty()) {
            outFile.write(data.c_str(), data.length());
          }
        }
      }

      if (sharedMemory_.isReaderDone() && sharedMemory_.isBufferFree()) {
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