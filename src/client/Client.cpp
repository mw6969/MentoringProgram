
#include "Client.h"
#include "Encoder.h"
#include "Reader.h"
#include "Sender.h"
#include "Utils.h"

#include <thread>
#include <tuple>

Client::Client(boost::asio::io_service &ioService, const unsigned short port)
    : socket_(ioService), endpointIterator_(tcp::resolver(ioService).resolve(
                              {"localhost", std::to_string(port)})) {
  Connect();
}

void Client::SendFiles(const std::vector<std::string> &files) {
  for (const auto &file : files) {
    auto sender = std::make_shared<Sender>(file, socket_);
    auto encoder = std::make_shared<Encoder>();
    auto reader = std::make_shared<Reader>(file);

    sender->SetEncoder(encoder);
    encoder->SetReader(reader);
    encoder->SetSender(sender);
    reader->SetEncoder(encoder);

    std::apply(
        [&](auto &&...args) {
          return std::make_tuple(std::jthread(
              &std::remove_reference_t<decltype(*args)>::ProcessData, args)...);
        },
        std::make_tuple(sender, encoder, reader));
  }
}

void Client::Connect() {
  boost::system::error_code ec;
  boost::asio::connect(socket_, endpointIterator_, ec);
  if (ec) {
    throw std::runtime_error("Client has failed to connect: " + ec.message());
  }
}