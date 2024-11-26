#ifndef _ENCODER_
#define _ENCODER_

#include "Cryptor.h"
#include "Processor.h"
#include "Reader.h"
#include "Sender.h"

#include <memory>
#include <tuple>

class Reader;
class Sender;

// Format of DataType is buffer/length/size
class Encoder
    : public Processor<Encoder, std::tuple<std::vector<char>, std::size_t,
                                           std::streamsize>> {
public:
  Encoder();
  void SetReader(const std::shared_ptr<Reader> &reader);
  void SetSender(const std::shared_ptr<Sender> &sender);
  void Initialize();
  bool IsDone();
  void ProcessDataImpl();
  void Finalize();

private:
  std::shared_ptr<Reader> reader_;
  std::shared_ptr<Sender> sender_;
  std::unique_ptr<Cryptor> cryptor_;
};

#endif