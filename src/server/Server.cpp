#include "Server.h"
#include "DataWriter.h"

Server::Server(boost::asio::io_service &ioService, const unsigned short port)
    : acceptor_(ioService, tcp::endpoint(tcp::v4(), port)) {
  accept();
}

void Server::accept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          std::make_shared<DataWriter>(std::move(socket))->processData("");
        } else {
          throw std::runtime_error("Server has failed to accept connection: " +
                                   ec.message());
        }
        accept();
      });
}