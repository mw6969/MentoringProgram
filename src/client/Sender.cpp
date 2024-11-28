
#include "Sender.h"
#include "Utils.h"

Sender::Sender(const std::string &filename, tcp::socket &socket)
    : filename_(filename), socket_(socket) {}

void Sender::SetEncoder(const std::shared_ptr<Encoder> &encoder) {
  encoder_ = encoder;
}

void Sender::Initialize() {
  auto data = encoder_->Pop();

  // Send padding length
  const uint32_t networkPaddingLength =
      htonl(static_cast<uint32_t>(Utils::PaddingLength(data.size)));

  boost::asio::write(
      socket_,
      boost::asio::buffer(&networkPaddingLength, sizeof(networkPaddingLength)));
  data.size += Utils::PaddingLength(data.size);

  // Send length of filename
  const uint32_t nameLength = filename_.length();
  boost::asio::write(socket_,
                     boost::asio::buffer(&nameLength, sizeof(nameLength)));

  // Send name of file
  boost::asio::write(socket_, boost::asio::buffer(filename_));

  // Send size of file
  boost::asio::write(socket_,
                     boost::asio::buffer(&data.size, sizeof(data.size)));

  // Send first buffer
  boost::asio::write(socket_,
                     boost::asio::buffer(data.buffer.data(), data.length));
}

bool Sender::IsDone() {
  return encoder_->NotifyComplete() && encoder_->Empty();
}

bool Sender::ReadyToProcessData() { return !encoder_->Empty(); }

void Sender::ProcessDataImpl() {
  auto [srcBuffer, srcLength, srcSize] = encoder_->Pop();
  boost::asio::write(socket_, boost::asio::buffer(srcBuffer.data(), srcLength));
}

void Sender::Finalize() {
  const auto hash = Utils::getSha256Hash(filename_);

  // Send size of file hash
  const uint32_t hashSize = hash.size();
  boost::asio::write(socket_, boost::asio::buffer(&hashSize, sizeof(hashSize)));

  // Send file hash
  boost::asio::write(socket_, boost::asio::buffer(hash));
}