#include "CustomException.h"

const char *CustomException::what() const noexcept { return message_.c_str(); }