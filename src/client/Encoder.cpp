
#include "Encoder.h"

Encoder::Encoder() : cryptor_(std::make_unique<Cryptor>()) {}

void Encoder::SetReader(const std::shared_ptr<Reader> &reader) {
  reader_ = reader;
}

void Encoder::SetSender(const std::shared_ptr<Sender> &sender) {
  sender_ = sender;
}

void Encoder::Initialize() {}

bool Encoder::IsDone() { return reader_->NotifyComplete() && reader_->Empty(); }

bool Encoder::ReadyToProcessData() {
  return sender_->WaitNextData() && !reader_->Empty();
}

void Encoder::ProcessDataImpl() {
  auto [srcBuffer, srcLength, srcSize] = reader_->Pop();
  std::vector<char> encodedBuffer(srcBuffer.size());
  cryptor_->getEncryptor()->ProcessData(
      reinterpret_cast<CryptoPP::byte *>(encodedBuffer.data()),
      reinterpret_cast<CryptoPP::byte *>(srcBuffer.data()), srcLength);
  Push({std::move(encodedBuffer), srcLength, srcSize});
}

void Encoder::Finalize() {}