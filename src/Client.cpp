
#include "Client.h"
#include "Utils.h"

#include <fstream>

Client::Client(boost::asio::io_service &ioService, const std::string &host,
               const unsigned short port)
    : socket_(ioService), endpointIterator_(tcp::resolver(ioService).resolve(
                              {host, std::to_string(port)})),
      cryptor_(std::make_unique<Cryptor>()) {
  connect();
}

void Client::sendFiles(const std::vector<std::string> &files) {
  for (const auto &file : files) {
    sendFile(file);
  }
}

void Client::connect() {
  boost::system::error_code ec;
  boost::asio::connect(socket_, endpointIterator_, ec);
  if (ec) {
    throw std::runtime_error("Failed to connect: " + ec.message());
  }
}

void Client::sendFile(const std::string &fileName) {
  std::ifstream inputFile(fileName, std::ios_base::binary);
  if (!inputFile) {
    throw std::runtime_error("Failed to open file: " + fileName);
  }

  // Send public key
  CryptoPP::byte publicKey[CryptoPP::AES::DEFAULT_KEYLENGTH];
  cryptor_->generateKey(publicKey);
  boost::asio::write(socket_, boost::asio::buffer(
                                  publicKey, CryptoPP::AES::DEFAULT_KEYLENGTH));

  inputFile.seekg(0, std::ios::end);
  const std::streamsize size = inputFile.tellg();
  inputFile.seekg(0, std::ios::beg);

  // Send length of filename
  const uint32_t nameLength = fileName.length();
  boost::asio::write(socket_,
                     boost::asio::buffer(&nameLength, sizeof(nameLength)));

  // Send name of file
  boost::asio::write(socket_, boost::asio::buffer(fileName));

  // Send size of file
  boost::asio::write(socket_, boost::asio::buffer(&size, sizeof(size)));

  // Send file content
  char buf[Utils::BufferSize];
  while (inputFile) {
    inputFile.read(buf, sizeof(buf));
    if (const std::streamsize length = inputFile.gcount(); length > 0) {
      CryptoPP::byte encryptedBuf[sizeof(buf)];
      cryptor_->getEncryptor(publicKey)->ProcessData(
          encryptedBuf, reinterpret_cast<CryptoPP::byte *>(buf), length);
      boost::asio::write(socket_, boost::asio::buffer(encryptedBuf, length));
    }
  }

  const std::string hash = Cryptor::getSha256Hash(fileName);

  // Send size of file hash
  const uint32_t hashSize = htonl(hash.size());
  boost::asio::write(socket_, boost::asio::buffer(&hashSize, sizeof(hashSize)));

  // Send file hash
  boost::asio::write(socket_, boost::asio::buffer(hash));
}