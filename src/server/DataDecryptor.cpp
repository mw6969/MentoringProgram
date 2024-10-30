#include "DataDecryptor.h"

DataDecryptor::DataDecryptor() : cryptor_(std::make_unique<Cryptor>()) {}

void DataDecryptor::processData(char *originalBuf, CryptoPP::byte *decryptedBuf,
                                const std::streamsize length) {
  std::cout << "Decrypting data..." << std::endl;

  cryptor_->getDecryptor()->ProcessData(
      decryptedBuf, reinterpret_cast<CryptoPP::byte *>(originalBuf), length);

  notifyComplete();
  waitNextData();
}

void DataDecryptor::notifyComplete() {
  std::cout << "Decryption Complete." << std::endl;
}

void DataDecryptor::waitNextData() {
  std::cout << "Waiting for data to decrypt..." << std::endl;
}
