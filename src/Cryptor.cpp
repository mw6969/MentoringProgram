#include "Cryptor.h"

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