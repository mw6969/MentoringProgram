#ifndef _SERVER_
#define _SERVER_

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server {
public:
  Server(boost::asio::io_service &io_service, const unsigned short port);

private:
  void accept();

private:
  tcp::acceptor acceptor_;
};

#endif