#ifndef _CRYPTOR_
#define _CRYPTOR_

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>

class Cryptor {
public:
  Cryptor() = default;

  void generateKey(CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH]);
  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption *
  getEncryptor(const CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH]);
  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption *
  getDecryptor(const CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH]);
  static std::string getSha256Hash(const std::string &fileName);

private:
  CryptoPP::byte iv_[CryptoPP::AES::BLOCKSIZE];
  std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption> encryptor_;
  std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption> decryptor_;
};

#endif