#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Logger.h"

constexpr std::string fileName("log.txt");

Logger::Logger(const std::string &&message) {
  std::ofstream outFile(fileName, std::ios_base::app);
  if (!outFile.is_open()) {
    std::cerr << "Logger is not open\n";
    return;
  }
  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  outFile << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << " "
          << message << "\n";

  outFile.close();
}