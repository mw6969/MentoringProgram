#include "Cryptor.h"

#include <cryptopp/files.h>
#include <cryptopp/hex.h>

CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption *Cryptor::getEncryptor() {
  if (!encryptor_) {
    encryptor_ =
        std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption>(
            key_, sizeof(key_), iv_);
  }
  return encryptor_.get();
}

CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption *Cryptor::getDecryptor() {
  if (!decryptor_) {
    decryptor_ =
        std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption>(
            key_, sizeof(key_), iv_);
  }
  return decryptor_.get();
}

std::string Cryptor::getSha256Hash(const std::string &fileName) {
  CryptoPP::SHA256 hash;
  std::string digest;

  CryptoPP::FileSource f(
      fileName.c_str(), true,
      new CryptoPP::HashFilter(
          hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));

  return digest;
}