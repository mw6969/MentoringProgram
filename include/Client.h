#ifndef _CLIENT_
#define _CLIENT_

#include <boost/asio.hpp>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

class Client {
public:
  Client(boost::asio::io_service &ioService, const std::string &host,
         const unsigned short port);

  void sendFiles(const std::vector<std::string> &files);

private:
  void connect();
  void sendFile(const std::string &fileName);

private:
  tcp::socket socket_;
  tcp::resolver::iterator endpointIterator_;
};

#endif