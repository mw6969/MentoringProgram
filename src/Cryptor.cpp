#include "Cryptor.h"

void Cryptor::init() {
  encryptor_ = std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption>(
      key_, sizeof(key_), iv_);
  decryptor_ = std::make_unique<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption>(
      key_, sizeof(key_), iv_);
}

CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption *Cryptor::getEncryptor() {
  if (!encryptor_) {
    init();
  }
  return encryptor_.get();
}

CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption *Cryptor::getDecryptor() {
  if (!decryptor_) {
    init();
  }
  return decryptor_.get();
}

void Cryptor::clear() {
  encryptor_.release();
  decryptor_.release();
}
