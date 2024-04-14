#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <string>

constexpr short BuffersCount = 5;
constexpr short DataLength = 100;

struct Data {
  char size;
  char data[DataLength];
};

struct Buffer {
  short freeIndexes[BuffersCount];
  short readyForWriteIndexes[BuffersCount];
  Data data[BuffersCount];
  bool readerDone;
};
class SharedMemory {
public:
  SharedMemory();
  ~SharedMemory() = default;

  std::string getData(const short index) const;
  Data *getFreeBuffer();
  Data *getReadyForWriteBuffer();
  bool attemptRelease() const;
  void setReaderDone(const bool value);
  void destroy();

private:
  void setReadyForWriteBufferIndex(const short index);
  void setFirstFreeBufferIndex(const short index);
  bool isBufferFree() const;
  bool isReaderDone() const;

private:
  Buffer *buffer_;
  int id_;
};

#endif