#ifndef _SESSION_
#define _SESSION_

#include "Cryptor.h"
#include "Utils.h"

#include <boost/asio.hpp>
#include <fstream>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket socket);

  void start();

private:
  void readPublicKey();
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
  std::streamsize leftToRead_;
  std::string receivedHash_;
  std::string fileName_;
  std::ofstream outputFile_;
  std::unique_ptr<Cryptor> cryptor_;
  CryptoPP::byte publicKey_[CryptoPP::AES::DEFAULT_KEYLENGTH];
};

#endif