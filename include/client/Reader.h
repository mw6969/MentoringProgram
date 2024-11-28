#ifndef _READER_
#define _READER_

#include "ClientDataStruct.h"
#include "Encoder.h"
#include "Processor.h"

#include <fstream>
#include <memory>
#include <string>

class Encoder;

class Reader : public Processor<Reader, ClientDataStruct> {
public:
  Reader(const std::string &filename);
  void SetEncoder(const std::shared_ptr<Encoder> &encoder);
  void Initialize();
  bool IsDone();
  bool ReadyToProcessData();
  void ProcessDataImpl();
  void Finalize();

private:
  std::ifstream file_;
  std::string filename_;
  std::shared_ptr<Encoder> encoder_;
  std::streamsize size_;
};

#endif