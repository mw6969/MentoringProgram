#ifndef _UTILS_
#define _UTILS_

#include <boost/asio/io_service.hpp>
#include <string>

class Utils {
public:
  static std::string getUniqueFileName(const std::string &originName);
  static std::string getSha256Hash(const std::string &fileName);
  static bool HasPadding(const std::streamsize size);
  static std::streamsize PaddingLength(const std::streamsize size);

public:
  static boost::asio::io_service IOService;
  static const unsigned short BufferSize = 1024;
  static const unsigned short Port = 9999;
};

#endif