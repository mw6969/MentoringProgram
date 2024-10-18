#ifndef _SESSION_
#define _SESSION_

#include "Cryptor.h"
#include "Utils.h"

#include <fstream>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket socket);

  void start();

private:
  void readPaddingLength();
  void readFileNameLength();
  void readFileName();
  void readFileSize();
  void readFileContent();
  void readFileHashSize();
  void readFileHash();

private:
  tcp::socket socket_;
  char data_[Utils::BufferSize];
  uint32_t receivedHashSize_;
  uint32_t nameLength_;
  uint32_t paddingLength_;
  std::streamsize leftToRead_;
  std::string receivedHash_;
  std::string originFileName_;
  std::string uniqueFileName_;
  std::ofstream outputFile_;
  std::unique_ptr<Cryptor> cryptor_;
};

#endif