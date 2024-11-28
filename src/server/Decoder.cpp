
#include "Decoder.h"

Decoder::Decoder() : cryptor_(std::make_unique<Cryptor>()) {}

void Decoder::SetWriter(const std::shared_ptr<Writer> &writer) {
  writer_ = writer;
}

void Decoder::SetReceiver(const std::shared_ptr<Receiver> &receiver) {
  receiver_ = receiver;
}

void Decoder::Initialize() {}

bool Decoder::IsDone() {
  return receiver_->NotifyComplete() && receiver_->Empty();
}

bool Decoder::ReadyToProcessData() {
  return writer_->WaitNextData() && !receiver_->Empty();
}

void Decoder::ProcessDataImpl() {
  auto [srcBuffer, srcLength, srcPadding] = receiver_->Pop();
  std::vector<char> decodedBuffer(srcBuffer.size());
  cryptor_->getDecryptor()->ProcessData(
      reinterpret_cast<CryptoPP::byte *>(decodedBuffer.data()),
      reinterpret_cast<CryptoPP::byte *>(srcBuffer.data()), srcLength);
  Push({std::move(decodedBuffer), srcLength, srcPadding});
}

void Decoder::Finalize() {}