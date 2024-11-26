#ifndef _READER_
#define _READER_

#include "Encoder.h"
#include "Processor.h"

#include <fstream>
#include <memory>
#include <string>
#include <tuple>

class Encoder;

// Format of DataType is buffer/length/size
class Reader
    : public Processor<
          Reader, std::tuple<std::vector<char>, std::size_t, std::streamsize>> {
public:
  Reader(const std::string &filename);
  void SetEncoder(const std::shared_ptr<Encoder> &encoder);
  void Initialize();
  bool IsDone();
  void ProcessDataImpl();
  void Finalize();

private:
  std::ifstream file_;
  std::string filename_;
  std::shared_ptr<Encoder> encoder_;
  std::streamsize size_;
};

#endif