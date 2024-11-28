#include "Reader.h"
#include "Utils.h"

Reader::Reader(const std::string &filename) : filename_(filename) {}

void Reader::SetEncoder(const std::shared_ptr<Encoder> &encoder) {
  encoder_ = encoder;
}

void Reader::Initialize() {
  file_.open(filename_, std::ios::binary);
  if (!file_.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename_);
  }

  file_.seekg(0, std::ios::end);
  size_ = file_.tellg();
  file_.seekg(0, std::ios::beg);
}

bool Reader::IsDone() { return file_.eof(); }

bool Reader::ReadyToProcessData() { return encoder_->WaitNextData(); }

void Reader::ProcessDataImpl() {
  std::vector<char> buffer(Utils::BufferSize);
  file_.read(buffer.data(), buffer.size());
  if (file_.gcount() > 0) {
    auto length = file_.gcount();
    if ((length < Utils::BufferSize) && Utils::HasPadding(size_)) {
      length += Utils::PaddingLength(size_);
    }
    Push({std::move(buffer), length, size_});
  }
}

void Reader::Finalize() { file_.close(); }