
#include "Receiver.h"
#include "Utils.h"

Receiver::Receiver(tcp::socket socket) : socket_(std::move(socket)) {}

void Receiver::SetDecoder(const std::shared_ptr<Decoder> &decoder) {
  decoder_ = decoder;
}

void Receiver::Initialize() {
  paddingLength_ = 0;
  ReadPaddingLength();
}

bool Receiver::IsDone() { return leftToRead_ <= 0; }

bool Receiver::ReadyToProcessData() { return decoder_->WaitNextData(); }

void Receiver::ProcessDataImpl() {
  std::vector<char> buffer(
      std::min<std::streamsize>(Utils::BufferSize, leftToRead_));
  ReadData(buffer, [this, &buffer](size_t length) {
    const uint32_t paddingLength = leftToRead_ == length ? paddingLength_ : 0;
    leftToRead_ -= length;
    Push({std::move(buffer), length, paddingLength});
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