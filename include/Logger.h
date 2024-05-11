#ifndef _LOGGER_
#define _LOGGER_

#include <fstream>

class Logger {
public:
  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;

  static Logger &getInstance();
  std::ofstream &getStream();

private:
  Logger();
  ~Logger() = default;

private:
  std::ofstream logFile_;
};

#endif