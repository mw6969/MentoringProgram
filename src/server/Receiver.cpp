
#include "Receiver.h"
#include "Utils.h"

Receiver::Receiver(tcp::socket socket) : socket_(std::move(socket)) {}

void Receiver::Initialize() {
  paddingLength_ = 0;
  ReadPaddingLength();
}

bool Receiver::IsDone() { return leftToRead_ <= 0; }

void Receiver::ProcessDataImpl() {
  std::vector<char> buffer(
      std::min<std::streamsize>(Utils::BufferSize, leftToRead_));
  ReadData(buffer, [this, &buffer](size_t length) {
    std::get<0>(data_) = buffer;
    std::get<1>(data_) = length;
    std::get<2>(data_) = leftToRead_ == length ? paddingLength_ : 0;
    leftToRead_ -= length;
    Push(std::move(data_));
  });
}

void Receiver::Finalize() { ReadFileHashSize(); }

std::string Receiver::GetUniqueFileName() const { return uniqueFileName_; }

std::string Receiver::GetReceivedHash() const { return receivedHash_; }

void Receiver::ReadPaddingLength() {
  auto buffer = boost::asio::buffer(&paddingLength_, sizeof(paddingLength_));
  ReadData(buffer, [this](size_t length) {
    paddingLength_ = ntohl(paddingLength_);
    ReadFileNameLength();
  });
}

void Receiver::ReadFileNameLength() {
  auto buffer = boost::asio::buffer(&nameLength_, sizeof(nameLength_));
  ReadData(buffer, [this](size_t length) { ReadFileName(); });
}

void Receiver::ReadFileName() {
  originFileName_.resize(nameLength_);
  auto buffer = boost::asio::buffer(originFileName_);
  ReadData(buffer, [this](size_t length) {
    uniqueFileName_ = Utils::getUniqueFileName(originFileName_);
    ReadFileSize();
  });
}

void Receiver::ReadFileSize() {
  auto buffer = boost::asio::buffer(&leftToRead_, sizeof(leftToRead_));
  ReadData(buffer, [](size_t length) {});
}

void Receiver::ReadFileHashSize() {
  auto buffer =
      boost::asio::buffer(&receivedHashSize_, sizeof(receivedHashSize_));
  ReadData(buffer, [this](size_t length) { ReadFileHash(); });
}

void Receiver::ReadFileHash() {
  receivedHash_.resize(receivedHashSize_);
  auto buffer = boost::asio::buffer(receivedHash_);
  ReadData(buffer, [](size_t length) {});
}