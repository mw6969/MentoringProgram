
#include <fstream>
#include <iostream>

#include "Tool.h"

void Tool::reader(const std::string &fileName) {
  if (std::ifstream inFile{fileName.data(),
                           std::ios_base::in | std::ios_base::binary};
      inFile.is_open()) {
    std::cout << "Reader has started\n";

    while (inFile) {
      if (auto buffer = sharedMemory_.getFreeBuffer(); buffer != nullptr) {
        inFile.read(buffer->data, 99);
        buffer->size = inFile.gcount();
        sharedMemory_.pushToWriteQueue(buffer);
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
      if (auto buffer = sharedMemory_.popFromWriteQueue(); buffer != nullptr) {
        outFile.write(buffer->data, buffer->size);
        sharedMemory_.releaseBuffer(buffer);
      }
      if (sharedMemory_.attemptRelease()) {
        break;
      }
    }
    outFile.close();

    std::cout << "Writer has done\n";
  } else {
    std::cout << "Error opening output file\n";
  }
}