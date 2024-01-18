
#include <cstring>
#include <fstream>
#include <iostream>

#include "Tool.h"

void Tool::reader(const std::string &fileName) {
  std::ifstream inFile{fileName.data(),
                       std::ios_base::in | std::ios_base::binary};
  if (inFile.is_open()) {
    std::cout << "Reader has started\n";

    while (inFile) {
      ss.acquire();

      char buffer[100];
      inFile.read(buffer, 100);

      sharedMemory_.push(buffer);

      ss.release();
    }
    inFile.close();

    readerDone_ = true;

    std::cout << "Reader has done\n";
  } else {
    std::cout << "Error opening input file\n";
  }
}

void Tool::writer(const std::string &fileName) {
  std::ofstream outFile{fileName.data(), std::ios::app | std::ios_base::binary};
  if (outFile.is_open()) {
    std::cout << "Writer has started\n";

    while (true) {
      ss.acquire();

      std::string front{sharedMemory_.front()};
      sharedMemory_.popFront();

      ss.release();

      outFile.write(front.c_str(), front.length());

      if (readerDone_ && sharedMemory_.empty()) {
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
