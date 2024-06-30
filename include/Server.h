#ifndef _SERVER_
#define _SERVER_

#include "Cryptor.h"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server {
public:
  Server(boost::asio::io_service &io_service, const unsigned short port,
         const std::shared_ptr<Cryptor> &cryptor);

private:
  void accept();

private:
  tcp::acceptor acceptor_;
  std::shared_ptr<Cryptor> cryptor_;
};

#endif