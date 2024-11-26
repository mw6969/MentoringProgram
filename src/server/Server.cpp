#include "Server.h"
#include "Decoder.h"
#include "Receiver.h"
#include "Writer.h"

#include <thread>
#include <tuple>

Server::Server(boost::asio::io_service &ioService, const unsigned short port)
    : acceptor_(ioService, tcp::endpoint(tcp::v4(), port)) {
  accept();
}

void Server::accept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          auto writer = std::make_shared<Writer>();
          auto decoder = std::make_shared<Decoder>();
          auto receiver = std::make_shared<Receiver>(std::move(socket));

          decoder->SetReceiver(receiver);
          decoder->SetWriter(writer);
          writer->SetDecoder(decoder);
          writer->SetReceiver(receiver);

          std::apply(
              [&](auto &&...args) {
                return std::make_tuple(std::jthread(
                    &std::remove_reference_t<decltype(*args)>::ProcessData,
                    args)...);
              },
              std::make_tuple(writer, decoder, receiver));
        } else {
          throw std::runtime_error("Server has failed to accept connection: " +
                                   ec.message());
        }
        accept();
      });
}