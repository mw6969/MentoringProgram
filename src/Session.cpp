#include "Session.h"
#include "Cryptor.h"

Session::Session(tcp::socket socket)
    : socket_(std::move(socket)), cryptor_(std::make_unique<Cryptor>()) {}

void Session::start() {
  paddingLength_ = 0;
  readPaddingLength();
}

void Session::readPaddingLength() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(&paddingLength_, sizeof(paddingLength_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          paddingLength_ = ntohl(paddingLength_);
          readFileNameLength();
        } else if (ec != boost::asio::error::eof) {
          throw std::runtime_error(
              "Session has failed to read padding length: " + ec.message());
        }
      });
}

void Session::readFileNameLength() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(&nameLength_, sizeof(nameLength_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          readFileName();
        } else if (ec != boost::asio::error::eof) {
          throw std::runtime_error(
              "Session has failed to read file name length: " + ec.message());
        }
      });
}

void Session::readFileName() {
  originFileName_.resize(nameLength_);
  boost::asio::async_read(
      socket_, boost::asio::buffer(originFileName_),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          readFileSize();
        } else if (ec != boost::asio::error::eof) {
          throw std::runtime_error("Session has failed to read file name: " +
                                   ec.message());
        }
      });
}

void Session::readFileSize() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(&leftToRead_, sizeof(leftToRead_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          uniqueFileName_ = Utils::getUniqueFileName(originFileName_);
          outputFile_.open(uniqueFileName_, std::ios_base::binary);
          if (!outputFile_) {
            throw std::runtime_error("Session has failed to open file: " +
                                     uniqueFileName_);
          }
          readFileContent();
        } else if (ec != boost::asio::error::eof) {
          throw std::runtime_error("Session has failed to read file size: " +
                                   ec.message());
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

          // Subtract padding length if exist for the last block of data
          if ((leftToRead_ - length <= 0) && (paddingLength_ > 0)) {
            outputFile_.write(reinterpret_cast<const char *>(decryptedBuf),
                              length - paddingLength_);
          } else {
            outputFile_.write(reinterpret_cast<const char *>(decryptedBuf),
                              length);
          }
          leftToRead_ -= length;
          if (leftToRead_ > 0) {
            readFileContent();
          } else {
            outputFile_.close();
            readFileHashSize();
          }
        } else if (ec != boost::asio::error::eof) {
          throw std::runtime_error("Session has failed to read file content: " +
                                   ec.message());
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
          readFileHash();
        } else if (ec != boost::asio::error::eof) {
          throw std::runtime_error(
              "Session has failed to read file hash size: " + ec.message());
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
          // Check file integrity
          const auto hash = Cryptor::getSha256Hash(uniqueFileName_);
          if (receivedHash_ != hash) {
            throw std::runtime_error(
                "Session has failed to transmission file:  " + uniqueFileName_);
          }

          readPaddingLength();
        } else if (ec != boost::asio::error::eof) {
          throw std::runtime_error("Session has failed to read file hash: " +
                                   ec.message());
        }
      });
}