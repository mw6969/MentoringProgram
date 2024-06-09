#ifndef _SESSION_
#define _SESSION_

#include "Utils.h"

#include <boost/asio.hpp>
#include <fstream>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket socket);

  void start();

private:
  void readFileNameLength();
  void readFileName();
  void readFileNameSize();
  void readFileContent();

private:
  tcp::socket socket_;
  char data_[Utils::BufferSize];
  uint32_t nameLength_;
  std::streamsize fileSize_;
  std::string fileName_;
  std::ofstream outputFile_;
};

#endif