#ifndef _CUSTOMEXCEPTION_
#define _CUSTOMEXCEPTION_

#include <stdexcept>

class CustomException : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

#endif