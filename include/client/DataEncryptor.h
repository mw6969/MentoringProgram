#ifndef _DATAENCRYPTOR_
#define _DATAENCRYPTOR_

#include "Cryptor.h"
#include "DataProcessor.h"

class DataEncryptor : public DataProcessor<DataEncryptor> {
public:
  DataEncryptor();

  void processData(char *originalBuf, CryptoPP::byte *encryptedBuf,
                   const std::streamsize length);

private:
  void notifyComplete();
  void waitNextData();

private:
  std::unique_ptr<Cryptor> cryptor_;
};

#endif