#ifndef _WRITER_
#define _WRITER_

#include "Decoder.h"
#include "Processor.h"
#include "Receiver.h"
#include "ServerDataStruct.h"

#include <fstream>
#include <memory>

class Decoder;
class Receiver;

class Writer : public Processor<Writer, ServerDataStruct> {
public:
  Writer() = default;
  void SetDecoder(const std::shared_ptr<Decoder> &decoder);
  void SetReceiver(const std::shared_ptr<Receiver> &receiver);
  void Initialize();
  bool IsDone();
  bool ReadyToProcessData();
  void ProcessDataImpl();
  void Finalize();

private:
  std::ofstream file_;
  std::shared_ptr<Decoder> decoder_;
  std::shared_ptr<Receiver> receiver_;
  std::streamsize size_;
};

#endif