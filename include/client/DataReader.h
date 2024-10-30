#ifndef _DATAREADER_
#define _DATAREADER_

#include "DataEncryptor.h"
#include "DataProcessor.h"

class DataReader : public DataProcessor<DataReader> {
public:
  DataReader(tcp::socket &socket);

  void processData(const std::string &fileName);

private:
  void notifyComplete();
  void waitNextData();

private:
  std::unique_ptr<DataEncryptor> dataEncryptor_;
  tcp::socket &socket_;
};

#endif