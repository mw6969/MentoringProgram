#ifndef _CLIENT_
#define _CLIENT_

#include "DataReader.h"

#include <boost/asio.hpp>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

class Client {
public:
  Client(boost::asio::io_service &ioService, const unsigned short port);

  void sendFiles(const std::vector<std::string> &files);

private:
  void connect();

private:
  tcp::socket socket_;
  tcp::resolver::iterator endpointIterator_;
  std::unique_ptr<DataReader> dataReader_;
};

#endif