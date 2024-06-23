#include "Session.h"

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() { readFileNameLength(); }

void Session::readFileNameLength() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(&nameLength_, sizeof(nameLength_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          readFileName();
        }
      });
}

void Session::readFileName() {
  fileName_.resize(nameLength_);
  boost::asio::async_read(
      socket_, boost::asio::buffer(fileName_),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          readFileSize();
        }
      });
}

void Session::readFileSize() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(&leftToRead_, sizeof(leftToRead_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          outputFile_.open(Utils::getUniqueFileName(fileName_),
                           std::ios_base::binary);
          readFileContent();
        }
      });
}

void Session::readFileContent() {
  boost::asio::async_read(
      socket_,
      boost::asio::buffer(
          data_, std::min(std::streamsize(Utils::BufferSize), leftToRead_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t length) {
        if (!ec && outputFile_) {
          outputFile_.write(data_, length);
          leftToRead_ -= length;
          if (leftToRead_ > 0) {
            readFileContent();
          } else {
            // Close file and get ready to read the next header
            outputFile_.close();
            readFileNameLength();
          }
        }
      });
}