#include "Utils.h"

#include <filesystem>

std::string Utils::getUniqueFileName(const std::string &originName) {
  std::string fileName;
  int counter = 0;
  while (true) {
    fileName = "out" + std::to_string(counter) + "_" + originName;
    if (std::filesystem::exists(fileName)) {
      counter++;
    } else {
      break;
    }
  }
  return fileName;
}