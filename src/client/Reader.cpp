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

void Reader::ProcessDataImpl() {
  if (!encoder_->WaitNextData()) {
    return;
  }

  std::tuple<std::vector<char>, std::size_t, uint32_t> data;
  std::get<0>(data).resize(Utils::BufferSize);
  std::get<2>(data) = size_;

  file_.read(std::get<0>(data).data(), std::get<0>(data).size());
  if (file_.gcount() > 0) {
    std::streamsize length = file_.gcount();
    if ((length < Utils::BufferSize) && Utils::HasPadding(size_)) {
      length += Utils::PaddingLength(size_);
    }
    std::get<1>(data) = length;
    Push(std::move(data));
  }
}

void Reader::Finalize() { file_.close(); }