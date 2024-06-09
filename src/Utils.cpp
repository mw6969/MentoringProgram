#include "Utils.h"

#include <fstream>

std::string Utils::getUniqueFileName(const std::string &originName) {
  std::string fileName;
  std::ifstream file;
  int counter = 0;
  while (true) {
    fileName = "out" + std::to_string(counter) + "_" + originName;
    file.open(fileName.c_str(), std::ios::in);
    if (file.is_open()) {
      counter++;
      file.close();
    } else {
      break;
    }
  }
  return fileName;
}