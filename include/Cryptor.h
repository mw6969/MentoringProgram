#ifndef _CRYPTOR_
#define _CRYPTOR_

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>

class Cryptor {
public:
  Cryptor() = default;

  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption *getEncryptor();
  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption *getDecryptor();
  static std::string getSha256Hash(const std::string &fileName);

private:
  CryptoPP::byte key_[CryptoPP::AES::DEFAULT_KEYLENGTH];
  CryptoPP::byte iv_[CryptoPP::AES::BLOCKSIZE];
  std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption> encryptor_;
  std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption> decryptor_;
};

#endif