
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

void Decoder::ProcessDataImpl() {
  if (!writer_->WaitNextData() || receiver_->Empty()) {
    return;
  }
  auto data = receiver_->Pop();
  std::tuple<std::vector<char>, std::size_t, uint32_t> decodedData;
  std::get<0>(decodedData).resize(std::get<0>(data).size());
  std::get<1>(decodedData) = std::get<1>(data);
  std::get<2>(decodedData) = std::get<2>(data);
  cryptor_->getDecryptor()->ProcessData(
      reinterpret_cast<CryptoPP::byte *>(std::get<0>(decodedData).data()),
      reinterpret_cast<CryptoPP::byte *>(std::get<0>(data).data()),
      std::get<1>(data));
  Push(std::move(decodedData));
}

void Decoder::Finalize() {}