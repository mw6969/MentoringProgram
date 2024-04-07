
#include <fstream>
#include <iostream>

#include "Tool.h"

const char *name = "my_mutex_qwe10";

void Tool::reader(const std::string &fileName) {
  if (std::ifstream inFile{fileName.data(),
                           std::ios_base::in | std::ios_base::binary};
      inFile.is_open()) {
    std::cout << "Reader has started\n";

    while (inFile) {
      NamedMutex namedMutex(name);
      namedMutex.lock();
      if (short index = sharedMemory_.getFreeBufferIndex(); index != -1) {
        char buffer[100] = {0};
        inFile.read(buffer, sizeof(buffer) - 1);
        sharedMemory_.setData(index, buffer, inFile.gcount());
      }
      namedMutex.unlock();
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
        NamedMutex namedMutex(name);
        namedMutex.lock();
        if (short index = sharedMemory_.getReadyForWriteBufferIndex();
            index != -1) {
          if (std::string data = sharedMemory_.getData(index); !data.empty()) {
            outFile.write(data.c_str(), data.length());
          }
        }
        namedMutex.unlock();
      }

      if (sharedMemory_.isReaderDone() && sharedMemory_.isBufferFree()) {
        shm_unlink(name);
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