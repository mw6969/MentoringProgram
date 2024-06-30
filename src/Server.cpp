#include "Server.h"
#include "Session.h"

Server::Server(boost::asio::io_service &ioService, const unsigned short port)
    : acceptor_(ioService, tcp::endpoint(tcp::v4(), port)) {
  accept();
}

void Server::accept() {
  acceptor_.async_accept([this](boost::system::error_code ec,
                                tcp::socket socket) {
    if (!ec) {
      std::make_shared<Session>(std::move(socket))->start();
    } else {
      throw std::runtime_error("Failed to accept connection: " + ec.message());
    }
    accept();
  });
}