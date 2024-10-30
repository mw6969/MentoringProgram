
#include "DataEncryptor.h"

DataEncryptor::DataEncryptor() : cryptor_(std::make_unique<Cryptor>()) {}

void DataEncryptor::processData(char *originalBuf, CryptoPP::byte *encryptedBuf,
                                const std::streamsize length) {
  std::cout << "Encrypting data..." << std::endl;

  cryptor_->getEncryptor()->ProcessData(
      encryptedBuf, reinterpret_cast<CryptoPP::byte *>(originalBuf), length);

  notifyComplete();
  waitNextData();
}

void DataEncryptor::notifyComplete() {
  std::cout << "Encryption Complete." << std::endl;
}

void DataEncryptor::waitNextData() {
  std::cout << "Waiting for data to encrypt..." << std::endl;
}