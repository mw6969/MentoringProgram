
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

void Encoder::ProcessDataImpl() {
  if (!sender_->WaitNextData()) {
    return;
  }
  auto data = reader_->Pop();
  std::tuple<std::vector<char>, std::size_t, std::streamsize> encodedData;
  std::get<0>(encodedData).resize(std::get<0>(data).size());
  std::get<1>(encodedData) = std::get<1>(data);
  std::get<2>(encodedData) = std::get<2>(data);
  cryptor_->getEncryptor()->ProcessData(
      reinterpret_cast<CryptoPP::byte *>(std::get<0>(encodedData).data()),
      reinterpret_cast<CryptoPP::byte *>(std::get<0>(data).data()),
      std::get<1>(data));
  Push(std::move(encodedData));
}

void Encoder::Finalize() {}