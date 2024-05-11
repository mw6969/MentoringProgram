#ifndef _CUSTOMEXCEPTION_
#define _CUSTOMEXCEPTION_

#include <exception>
#include <string>

class CustomException : public std::exception {
public:
  template <typename T>
  CustomException(T &&message) : message_(std::forward<T>(message)) {}

  const char *what() const noexcept override;
  static void handler();

private:
  std::string message_;
};

#endif