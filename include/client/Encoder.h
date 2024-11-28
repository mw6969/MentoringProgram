#ifndef _ENCODER_
#define _ENCODER_

#include "ClientDataStruct.h"
#include "Cryptor.h"
#include "Processor.h"
#include "Reader.h"
#include "Sender.h"

#include <memory>

class Reader;
class Sender;
class Encoder : public Processor<Encoder, ClientDataStruct> {
public:
  Encoder();
  void SetReader(const std::shared_ptr<Reader> &reader);
  void SetSender(const std::shared_ptr<Sender> &sender);
  void Initialize();
  bool IsDone();
  bool ReadyToProcessData();
  void ProcessDataImpl();
  void Finalize();

private:
  std::shared_ptr<Reader> reader_;
  std::shared_ptr<Sender> sender_;
  std::unique_ptr<Cryptor> cryptor_;
};

#endif