#ifndef _DECODER_
#define _DECODER_

#include "Cryptor.h"
#include "Processor.h"
#include "Receiver.h"
#include "ServerDataStruct.h"
#include "Writer.h"

#include <memory>

class Writer;
class Receiver;

class Decoder : public Processor<Decoder, ServerDataStruct> {
public:
  Decoder();
  void SetWriter(const std::shared_ptr<Writer> &writer);
  void SetReceiver(const std::shared_ptr<Receiver> &receiver);
  void Initialize();
  bool IsDone();
  bool ReadyToProcessData();
  void ProcessDataImpl();
  void Finalize();

private:
  std::shared_ptr<Writer> writer_;
  std::shared_ptr<Receiver> receiver_;
  std::unique_ptr<Cryptor> cryptor_;
};

#endif