#ifndef _DECODER_
#define _DECODER_

#include "Cryptor.h"
#include "Processor.h"
#include "Receiver.h"
#include "Writer.h"

#include <memory>
#include <tuple>

class Writer;
class Receiver;

// Format of DataType is buffer/length/padding
class Decoder
    : public Processor<Decoder,
                       std::tuple<std::vector<char>, std::size_t, uint32_t>> {
public:
  Decoder();
  void SetWriter(const std::shared_ptr<Writer> &writer);
  void SetReceiver(const std::shared_ptr<Receiver> &receiver);
  void Initialize();
  bool IsDone();
  void ProcessDataImpl();
  void Finalize();

private:
  std::shared_ptr<Writer> writer_;
  std::shared_ptr<Receiver> receiver_;
  std::unique_ptr<Cryptor> cryptor_;
};

#endif