#ifndef _RECEIVER_
#define _RECEIVER_

#include "Processor.h"
#include "Reader.h"

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Format of DataType is buffer/length/padding
class Receiver
    : public Processor<Receiver,
                       std::tuple<std::vector<char>, std::size_t, uint32_t>>,
      public std::enable_shared_from_this<Receiver> {
public:
  Receiver(tcp::socket socket);
  std::string GetUniqueFileName() const;
  std::string GetReceivedHash() const;
  void Initialize();
  bool IsDone();
  void ProcessDataImpl();
  void Finalize();

private:
  void ReadPaddingLength();
  void ReadFileNameLength();
  void ReadFileName();
  void ReadFileSize();
  void ReadFileHashSize();
  void ReadFileHash();

private:
  template <typename BufferType>
  void ReadData(BufferType &buffer, std::function<void(size_t)> onSuccess) {
    boost::system::error_code ec;
    size_t length;
    if constexpr (std::is_same_v<BufferType, std::vector<char>>) {
      length = boost::asio::read(
          socket_, boost::asio::buffer(buffer.data(), buffer.size()), ec);
    } else {
      length = boost::asio::read(socket_, buffer, ec);
    }
    if (!ec) {
      onSuccess(length);
    } else if (ec != boost::asio::error::eof) {
      throw std::runtime_error("Session has failed: " + ec.message());
    }
  }

private:
  std::tuple<std::vector<char>, std::size_t, uint32_t> data_;
  uint32_t paddingLength_;
  uint32_t nameLength_;
  uint32_t receivedHashSize_;
  std::streamsize leftToRead_{1};
  tcp::socket socket_;
  std::string originFileName_;
  std::string uniqueFileName_;
  std::string receivedHash_;
};

#endif