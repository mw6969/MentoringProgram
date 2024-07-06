#include "Session.h"
#include "Cryptor.h"

Session::Session(tcp::socket socket)
    : socket_(std::move(socket)), cryptor_(std::make_unique<Cryptor>()) {}

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
          CryptoPP::byte decryptedBuf[sizeof(data_)];
          cryptor_->getDecryptor()->ProcessData(
              decryptedBuf, reinterpret_cast<CryptoPP::byte *>(data_), length);
          outputFile_.write(reinterpret_cast<const char *>(decryptedBuf),
                            length);
          leftToRead_ -= length;
          if (leftToRead_ > 0) {
            readFileContent();
          } else {
            outputFile_.close();
            readFileHashSize();
          }
        }
      });
}

void Session::readFileHashSize() {
  boost::asio::async_read(
      socket_,
      boost::asio::buffer(&receivedHashSize_, sizeof(receivedHashSize_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          receivedHashSize_ = ntohl(receivedHashSize_);
          readFileHash();
        }
      });
}

void Session::readFileHash() {
  receivedHash_.resize(receivedHashSize_);
  boost::asio::async_read(
      socket_, boost::asio::buffer(receivedHash_),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          receivedHash_[receivedHash_.size()] = '\0';

          // Check file integrity
          const auto test = Cryptor::getSha256Hash(fileName_);
          if (receivedHash_ != test) {
            throw std::runtime_error("Failed to transmission file:  " +
                                     fileName_);
          }

          readFileNameLength();
        }
      });
}