
#include "DataWriter.h"

DataWriter::DataWriter(tcp::socket socket)
    : dataDecryptor_(std::make_unique<DataDecryptor>()),
      socket_(std::move(socket)) {}

void DataWriter::processData(const std::string &fileName) {
  paddingLength_ = 0;
  readPaddingLength();

  notifyComplete();
  waitNextData();
}

void DataWriter::notifyComplete() {
  std::cout << "File write complete." << std::endl;
}

void DataWriter::waitNextData() {
  std::cout << "Waiting for new file..." << std::endl;
}

void DataWriter::readPaddingLength() {
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

void DataWriter::readFileNameLength() {
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

void DataWriter::readFileName() {
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

void DataWriter::readFileSize() {
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

void DataWriter::readFileContent() {
  boost::asio::async_read(
      socket_,
      boost::asio::buffer(
          data_, std::min(std::streamsize(Utils::BufferSize), leftToRead_)),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t length) {
        if (!ec && outputFile_) {
          CryptoPP::byte decryptedBuf[sizeof(data_)];
          dataDecryptor_->processData(data_, decryptedBuf, length);

          assert(leftToRead_ >= length);
          const auto padding = leftToRead_ == length ? paddingLength_ : 0;
          outputFile_.write(reinterpret_cast<const char *>(decryptedBuf),
                            length - padding);

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

void DataWriter::readFileHashSize() {
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

void DataWriter::readFileHash() {
  receivedHash_.resize(receivedHashSize_);
  boost::asio::async_read(
      socket_, boost::asio::buffer(receivedHash_),
      [this, self = shared_from_this()](boost::system::error_code ec,
                                        std::size_t /*length*/) {
        if (!ec) {
          // Check file integrity
          const auto hash = Utils::getSha256Hash(uniqueFileName_);
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