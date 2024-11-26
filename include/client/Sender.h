#ifndef _SENDER_
#define _SENDER_

#include "Encoder.h"
#include "Processor.h"
#include "Reader.h"

#include <memory>
#include <string>
#include <tuple>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Encoder;

// Format of DataType is buffer/length/size
class Sender
    : public Processor<
          Sender, std::tuple<std::vector<char>, std::size_t, std::streamsize>> {
public:
  Sender(const std::string &filename, tcp::socket &socket);
  void SetEncoder(const std::shared_ptr<Encoder> &encoder);
  void Initialize();
  bool IsDone();
  void ProcessDataImpl();
  void Finalize();

private:
  tcp::socket &socket_;
  std::string filename_;
  std::shared_ptr<Encoder> encoder_;
};

#endif