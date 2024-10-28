#ifndef _UTILS_
#define _UTILS_

#include <boost/asio/io_service.hpp>
#include <string>

class Utils {
public:
  static std::string getUniqueFileName(const std::string &originName);

public:
  static boost::asio::io_service IOService;
  static const unsigned short BufferSize = 1024;
  static const unsigned short Port = 9999;
};

#endif