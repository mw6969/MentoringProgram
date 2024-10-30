#include "DataReader.h"

DataReader::DataReader(tcp::socket &socket)
    : dataEncryptor_(std::make_unique<DataEncryptor>()), socket_(socket) {}

void DataReader::processData(const std::string &fileName) {
  std::ifstream inputFile(fileName, std::ios_base::binary);
  if (!inputFile) {
    throw std::runtime_error("Client has failed to open file: " + fileName);
  }

  std::cout << "Reading file... " << std::endl;

  inputFile.seekg(0, std::ios::end);
  std::streamsize size = inputFile.tellg();
  inputFile.seekg(0, std::ios::beg);

  // Process padding
  const bool hasPadding = size % CryptoPP::AES::BLOCKSIZE != 0;
  const std::streamsize paddingLength =
      hasPadding
          ? (CryptoPP::AES::BLOCKSIZE - (size % CryptoPP::AES::BLOCKSIZE))
          : 0;
  const uint32_t networkPaddingLength =
      htonl(static_cast<uint32_t>(paddingLength));

  // Send padding length
  boost::asio::write(
      socket_,
      boost::asio::buffer(&networkPaddingLength, sizeof(networkPaddingLength)));
  size += paddingLength;

  // Send length of filename
  const uint32_t nameLength = fileName.length();
  boost::asio::write(socket_,
                     boost::asio::buffer(&nameLength, sizeof(nameLength)));

  // Send name of file
  boost::asio::write(socket_, boost::asio::buffer(fileName));

  // Send size of file
  boost::asio::write(socket_, boost::asio::buffer(&size, sizeof(size)));

  // Send file content
  char buf[Utils::BufferSize];
  while (inputFile.read(buf, sizeof(buf)) || inputFile.gcount() > 0) {
    std::streamsize length = inputFile.gcount();
    // Increase length if size of file is not a multiple of 16
    if ((length < Utils::BufferSize) && hasPadding) {
      length += paddingLength;
    }
    CryptoPP::byte encryptedBuf[sizeof(buf)];
    dataEncryptor_->processData(buf, encryptedBuf, length);
    boost::asio::write(socket_, boost::asio::buffer(encryptedBuf, length));
  }

  const auto hash = Utils::getSha256Hash(fileName);

  // Send size of file hash
  const uint32_t hashSize = hash.size();
  boost::asio::write(socket_, boost::asio::buffer(&hashSize, sizeof(hashSize)));

  // Send file hash
  boost::asio::write(socket_, boost::asio::buffer(hash));

  notifyComplete();
  waitNextData();
}

void DataReader::notifyComplete() {
  std::cout << "File read complete." << std::endl;
}

void DataReader::waitNextData() {
  std::cout << "Waiting for new file..." << std::endl;
}