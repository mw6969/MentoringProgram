#include "Logger.h"
#include "CustomException.h"
#include <chrono>
#include <iomanip>

constexpr std::string fileName("log.txt");

Logger::Logger() : logFile_(fileName, std::ios_base::app) {
  if (!logFile_) {
    throw CustomException("Unable to open log file");
  }
}

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::log(const std::string &message) {
  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  logFile_ << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << " "
           << message << "\n";
}