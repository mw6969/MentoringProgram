
#include <fstream>

#include "CustomException.h"
#include "Logger.h"
#include "Tool.h"

void Tool::reader(const std::string &inputFile) {
  if (std::ifstream inFile{inputFile.data(),
                           std::ios_base::in | std::ios_base::binary};
      inFile.is_open()) {
    Logger::getInstance().getStream() << "Reader has started\n";
    while (inFile) {
      if (auto buffer = sharedMemory_.getFreeBuffer(); buffer != nullptr) {
        inFile.read(buffer->data, 99);
        buffer->size = inFile.gcount();
        sharedMemory_.pushToWriteQueue(buffer);
      }
    }
    inFile.close();
    sharedMemory_.setReaderDone(true);
    Logger::getInstance().getStream() << "Reader has done\n";
  } else {
    throw CustomException("Failed to open input file");
  }
}

void Tool::writer(const std::string &outputFile) {
  if (std::ofstream outFile{outputFile.data(), std::ios_base::binary};
      outFile.is_open()) {
    Logger::getInstance().getStream() << "Writer has started\n";
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
    Logger::getInstance().getStream() << "Writer has done\n";
  } else {
    throw CustomException("Failed to open output file");
  }
}