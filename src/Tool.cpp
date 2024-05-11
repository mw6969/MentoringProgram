
#include <fstream>

#include "CustomException.h"
#include "Logger.h"
#include "Tool.h"

void Tool::reader(const std::string &fileName) {
  if (std::ifstream inFile{fileName.data(),
                           std::ios_base::in | std::ios_base::binary};
      inFile.is_open()) {
    Logger log("Reader has started");

    while (inFile) {
      if (auto buffer = sharedMemory_.getFreeBuffer(); buffer != nullptr) {
        inFile.read(buffer->data, 99);
        buffer->size = inFile.gcount();
        sharedMemory_.pushToWriteQueue(buffer);
      }
    }
    inFile.close();
    sharedMemory_.setReaderDone(true);

    Logger log2("Reader has done");
  } else {
    throw CustomException("Failed to open input file");
  }
}

void Tool::writer(const std::string &fileName) {
  if (std::ofstream outFile{fileName.data(), std::ios_base::binary};
      outFile.is_open()) {
    Logger log("Writer has started");

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

    Logger log2("Writer has done");
  } else {
    throw CustomException("Failed to open output file");
  }
}