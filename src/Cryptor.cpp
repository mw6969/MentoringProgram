#include "Cryptor.h"

#include <cassert>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <fstream>

std::vector<CryptoPP::byte> Cryptor::getKey() {
  std::ifstream file("key.txt", std::ios::binary);
  if (!file) {
    throw std::runtime_error("Cryptor has failed to open file: key.txt");
  }

  file.seekg(0, std::ios::end);

  assert((file.tellg() == 16) && "Key must contains 16 characters");

  std::vector<CryptoPP::byte> bytes(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read((char *)&bytes[0], bytes.size());
  file.close();

  return bytes;
}

CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption *Cryptor::getEncryptor() {
  if (!encryptor_) {
    const auto key = getKey();
    encryptor_ =
        std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption>(
            key.data(), key.size(), iv_);
  }
  return encryptor_.get();
}

CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption *Cryptor::getDecryptor() {
  if (!decryptor_) {
    const auto key = getKey();
    decryptor_ =
        std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption>(
            key.data(), key.size(), iv_);
  }
  return decryptor_.get();
}