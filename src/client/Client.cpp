
#include "Client.h"

Client::Client(boost::asio::io_service &ioService, const unsigned short port)
    : socket_(ioService), endpointIterator_(tcp::resolver(ioService).resolve(
                              {"localhost", std::to_string(port)})),
      dataReader_(std::make_unique<DataReader>(socket_)) {
  connect();
}

void Client::sendFiles(const std::vector<std::string> &files) {
  for (const auto &file : files) {
    dataReader_->processData(file);
  }
}

void Client::connect() {
  boost::system::error_code ec;
  boost::asio::connect(socket_, endpointIterator_, ec);
  if (ec) {
    throw std::runtime_error("Client has failed to connect: " + ec.message());
  }
}