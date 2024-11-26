#ifndef _WRITER_
#define _WRITER_

#include "Decoder.h"
#include "Processor.h"
#include "Receiver.h"

#include <fstream>
#include <memory>
#include <tuple>

class Decoder;

// Format of DataType is buffer/length/padding
class Writer
    : public Processor<Writer,
                       std::tuple<std::vector<char>, std::size_t, uint32_t>> {
public:
  Writer() = default;
  void SetDecoder(const std::shared_ptr<Decoder> &decoder);
  void SetReceiver(const std::shared_ptr<Receiver> &receiver);
  void Initialize();
  bool IsDone();
  void ProcessDataImpl();
  void Finalize();

private:
  std::ofstream file_;
  std::shared_ptr<Decoder> decoder_;
  std::shared_ptr<Receiver> receiver_;
  std::streamsize size_;
};

#endif