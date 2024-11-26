#include "Writer.h"
#include "Utils.h"

void Writer::SetDecoder(const std::shared_ptr<Decoder> &decoder) {
  decoder_ = decoder;
}

void Writer::SetReceiver(const std::shared_ptr<Receiver> &receiver) {
  receiver_ = receiver;
}

void Writer::Initialize() {
  auto data = decoder_->Pop();
  const auto fileName = receiver_->GetUniqueFileName();
  file_.open(fileName, std::ios_base::binary);
  if (!file_.is_open()) {
    throw std::runtime_error("Failed to open file: " + fileName);
  }
  file_.write(reinterpret_cast<const char *>(std::get<0>(data).data()),
              std::get<1>(data) - std::get<2>(data));
}

bool Writer::IsDone() {
  return decoder_->NotifyComplete() && decoder_->Empty();
}

void Writer::ProcessDataImpl() {
  if (decoder_->Empty()) {
    return;
  }
  auto data = decoder_->Pop();
  file_.write(reinterpret_cast<const char *>(std::get<0>(data).data()),
              std::get<1>(data) - std::get<2>(data));
}

void Writer::Finalize() {
  file_.close();
  const auto fileName = receiver_->GetUniqueFileName();
  const auto hash = Utils::getSha256Hash(fileName);
  if (hash != receiver_->GetReceivedHash()) {
    throw std::runtime_error("File integrity mismatch for file: " + fileName);
  }
}