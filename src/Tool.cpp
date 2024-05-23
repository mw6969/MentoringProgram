
#include <fstream>

#include "CustomException.h"
#include "Tool.h"

Tool::Tool(Logger &logger) : logger_(logger) {}

void Tool::reader(const std::string &inputFile) {
  if (std::ifstream inFile{inputFile.data(),
                           std::ios_base::in | std::ios_base::binary};
      inFile.is_open()) {
    logger_.log("Reader has started");
    while (inFile) {
      if (auto buffer = sharedMemory_.getFreeBuffer(); buffer != nullptr) {
        inFile.read(buffer->data, 99);
        buffer->size = inFile.gcount();
        sharedMemory_.pushToWriteQueue(buffer);
      }
    }
    inFile.close();
    sharedMemory_.setReaderDone(true);
    logger_.log("Reader has done");
  } else {
    throw CustomException("Failed to open input file");
  }
}

void Tool::writer(const std::string &outputFile) {
  if (std::ofstream outFile{outputFile.data(), std::ios_base::binary};
      outFile.is_open()) {
    logger_.log("Writer has started");
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
    logger_.log("Writer has done");
  } else {
    throw CustomException("Failed to open output file");
  }
}