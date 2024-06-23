#include "Client.h"
#include "Utils.h"

#include <fstream>

Client::Client(boost::asio::io_service &ioService, const std::string &host,
               const unsigned short port)
    : socket_(ioService), endpointIterator_(tcp::resolver(ioService).resolve(
                              {host, std::to_string(port)})) {
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
      boost::asio::write(socket_, boost::asio::buffer(buf, length));
    }
  }
}