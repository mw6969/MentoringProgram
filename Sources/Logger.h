#ifndef _LOGGER_
#define _LOGGER_

#include <string>

class Logger {
public:
  Logger(const std::string &&message);
  ~Logger() = default;
};

#endif