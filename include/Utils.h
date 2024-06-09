#ifndef _UTILS_
#define _UTILS_

#include <string>

class Utils {
public:
  static std::string getUniqueFileName(const std::string &originName);

public:
  static const unsigned short BufferSize = 1024;
};

#endif