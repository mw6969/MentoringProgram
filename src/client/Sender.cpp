
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
      htonl(static_cast<uint32_t>(Utils::PaddingLength(std::get<2>(data))));

  boost::asio::write(
      socket_,
      boost::asio::buffer(&networkPaddingLength, sizeof(networkPaddingLength)));
  std::get<2>(data) += Utils::PaddingLength(std::get<2>(data));

  // Send length of filename
  const uint32_t nameLength = filename_.length();
  boost::asio::write(socket_,
                     boost::asio::buffer(&nameLength, sizeof(nameLength)));

  // Send name of file
  boost::asio::write(socket_, boost::asio::buffer(filename_));

  // Send size of file
  boost::asio::write(socket_, boost::asio::buffer(&std::get<2>(data),
                                                  sizeof(std::get<2>(data))));

  // Send first buffer
  boost::asio::write(socket_, boost::asio::buffer(std::get<0>(data).data(),
                                                  std::get<1>(data)));
}

bool Sender::IsDone() {
  return encoder_->NotifyComplete() && encoder_->Empty();
}

void Sender::ProcessDataImpl() {
  auto data = encoder_->Pop();
  boost::asio::write(socket_, boost::asio::buffer(std::get<0>(data).data(),
                                                  std::get<1>(data)));
}

void Sender::Finalize() {
  const auto hash = Utils::getSha256Hash(filename_);

  // Send size of file hash
  const uint32_t hashSize = hash.size();
  boost::asio::write(socket_, boost::asio::buffer(&hashSize, sizeof(hashSize)));

  // Send file hash
  boost::asio::write(socket_, boost::asio::buffer(hash));
}