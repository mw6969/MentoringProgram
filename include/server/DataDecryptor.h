#ifndef _DATADECRYPTOR_
#define _DATADECRYPTOR_

#include "Cryptor.h"
#include "DataProcessor.h"

class DataDecryptor : public DataProcessor<DataDecryptor> {
public:
  DataDecryptor();

  void processData(char *originalBuf, CryptoPP::byte *decryptedBuf,
                   const std::streamsize length);

private:
  void notifyComplete();
  void waitNextData();

private:
  std::unique_ptr<Cryptor> cryptor_;
};

#endif