#include "Logger.h"

#include <chrono>
#include <iomanip>
#include <iostream>

constexpr std::string fileName("log.txt");

Logger::Logger() : logFile_(fileName, std::ios_base::app) {}

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::log(const std::string &message) {
  if (logFile_) {
    auto now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile_ << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << " "
             << message << "\n";
  } else {
    std::cerr << "Logger is unavailable\n";
  }
}