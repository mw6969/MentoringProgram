#ifndef _LOGGER_
#define _LOGGER_

#include <fstream>
#include <string>

class Logger {
public:
  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;

  static Logger &getInstance();
  void log(const std::string &message);

private:
  Logger();
  ~Logger() = default;

private:
  std::ofstream logFile_;
};

#endif