#include "Utils.h"

#include <filesystem>

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>

boost::asio::io_service Utils::IOService;
const unsigned short Utils::BufferSize;
const unsigned short Utils::Port;

std::string Utils::getUniqueFileName(const std::string &originName) {
  std::string fileName;
  int counter = 0;
  while (true) {
    fileName = "out" + std::to_string(counter) + "_" + originName;
    if (std::filesystem::exists(fileName)) {
      counter++;
    } else {
      break;
    }
  }
  return fileName;
}

std::string Utils::getSha256Hash(const std::string &fileName) {
  CryptoPP::SHA256 hash;
  std::string digest;

  CryptoPP::FileSource f(
      fileName.c_str(), true,
      new CryptoPP ::HashFilter(
          hash, new CryptoPP::HexEncoder(new CryptoPP ::StringSink(digest))));

  return digest;
}

bool Utils::HasPadding(const std::streamsize size) {
  return size % CryptoPP::AES::BLOCKSIZE != 0;
}

std::streamsize Utils::PaddingLength(const std::streamsize size) {
  return HasPadding(size)
             ? (CryptoPP::AES::BLOCKSIZE - (size % CryptoPP::AES::BLOCKSIZE))
             : 0;
}