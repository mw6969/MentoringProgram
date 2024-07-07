#include "Cryptor.h"

#include <cryptopp/files.h>
#include <cryptopp/hex.h>

void Cryptor::generateKey(
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH]) {
  CryptoPP::AutoSeededRandomPool rng;
  rng.GenerateBlock(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
}

CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption *Cryptor::getEncryptor(
    const CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH]) {
  if (!encryptor_) {
    encryptor_ =
        std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption>(
            key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv_);
  }
  return encryptor_.get();
}

CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption *Cryptor::getDecryptor(
    const CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH]) {
  if (!decryptor_) {
    decryptor_ =
        std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption>(
            key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv_);
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