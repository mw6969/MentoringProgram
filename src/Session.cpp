#include "Session.h"

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() { readFileNameLength(); }

void Session::readFileNameLength() {
  auto self(shared_from_this());
  boost::asio::async_read(
      socket_, boost::asio::buffer(&nameLength_, sizeof(nameLength_)),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          readFileName();
        }
      });
}

void Session::readFileName() {
  auto self(shared_from_this());
  fileName_.resize(nameLength_);
  boost::asio::async_read(
      socket_, boost::asio::buffer(fileName_),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          readFileNameSize();
        }
      });
}

void Session::readFileNameSize() {
  auto self(shared_from_this());
  boost::asio::async_read(
      socket_, boost::asio::buffer(&fileSize_, sizeof(fileSize_)),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          outputFile_.open(Utils::getUniqueFileName(fileName_),
                           std::ios_base::binary);
          if (!outputFile_) {
            throw std::runtime_error("Failed to open file: " + fileName_);
          }
          readFileContent();
        } else {
          throw std::runtime_error("Failed to read file size: " + ec.message());
        }
      });
}

void Session::readFileContent() {
  auto self(shared_from_this());
  boost::asio::async_read(
      socket_,
      boost::asio::buffer(
          data_, std::min(std::streamsize(Utils::BufferSize), fileSize_)),
      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec && outputFile_) {
          outputFile_.write(data_, length);
          fileSize_ -= length;
          if (fileSize_ > 0) {
            readFileContent();
          } else {
            // Close file and get ready to read the next header
            outputFile_.close();
            readFileNameLength();
          }
        }
      });
}