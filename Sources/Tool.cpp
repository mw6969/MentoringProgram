
#include <fstream>
#include <iostream>

#include "NamedMutex.h"
#include "Tool.h"

const char *name = "my_mutex_qwe1012";

void Tool::reader(const std::string &fileName) {
  if (std::ifstream inFile{fileName.data(),
                           std::ios_base::in | std::ios_base::binary};
      inFile.is_open()) {
    std::cout << "Reader has started\n";

    while (inFile) {
      NamedMutex namedMutex(name);
      namedMutex.lock();
      if (auto buffer = sharedMemory_.getFreeBuffer(); buffer != nullptr) {
        inFile.read(buffer->data, 99);
        buffer->size = inFile.gcount();
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
        if (auto buffer = sharedMemory_.getReadyForWriteBuffer();
            buffer != nullptr) {
          outFile.write(buffer->data, buffer->size);
        }
        namedMutex.unlock();
      }

      if (sharedMemory_.attemptRelease()) {
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