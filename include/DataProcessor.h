#ifndef _DATAPROCESSOR_
#define _DATAPROCESSOR_

#include "Utils.h"

#include <fstream>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>

using boost::asio::ip::tcp;

template <typename T> class DataProcessor {
public:
  void processData(const std::string &data) {
    static_cast<T *>(this)->processData(data);
  }

  void processData(char *originalBuf, CryptoPP::byte *encryptedBuf,
                   const std::streamsize length) {
    static_cast<T *>(this)->processData(originalBuf, encryptedBuf, length);
  }

private:
  void notifyComplete() { static_cast<T *>(this)->notifyComplete(); }

  void waitNextData() { static_cast<T *>(this)->waitNextData(); }
};

#endif