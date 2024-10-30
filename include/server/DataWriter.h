#ifndef _DATAWRITER_
#define _DATAWRITER_

#include "DataDecryptor.h"
#include "DataProcessor.h"

class DataWriter : public DataProcessor<DataWriter>,
                   public std::enable_shared_from_this<DataWriter> {
public:
  DataWriter(tcp::socket socket);

  void processData(const std::string &fileName);

private:
  void notifyComplete();
  void waitNextData();

  void readPaddingLength();
  void readFileNameLength();
  void readFileName();
  void readFileSize();
  void readFileContent();
  void readFileHashSize();
  void readFileHash();

private:
  char data_[Utils::BufferSize];
  uint32_t receivedHashSize_;
  uint32_t nameLength_;
  uint32_t paddingLength_;
  std::streamsize leftToRead_;
  std::string receivedHash_;
  std::string originFileName_;
  std::string uniqueFileName_;
  std::ofstream outputFile_;
  std::unique_ptr<DataDecryptor> dataDecryptor_;
  tcp::socket socket_;
};

#endif