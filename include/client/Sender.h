#ifndef _SENDER_
#define _SENDER_

#include "ClientDataStruct.h"
#include "Encoder.h"
#include "Processor.h"
#include "Reader.h"

#include <memory>
#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Encoder;

class Sender : public Processor<Sender, ClientDataStruct> {
public:
  Sender(const std::string &filename, tcp::socket &socket);
  void SetEncoder(const std::shared_ptr<Encoder> &encoder);
  void Initialize();
  bool IsDone();
  bool ReadyToProcessData();
  void ProcessDataImpl();
  void Finalize();

private:
  tcp::socket &socket_;
  std::string filename_;
  std::shared_ptr<Encoder> encoder_;
};

#endif