#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include "NamedMutex.h"

constexpr short BuffersCount = 5;
constexpr short DataLength = 100;

struct Data {
  size_t size;
  char data[DataLength];
};

struct Buffer {
  short freeIndexes[BuffersCount];
  Data *writeQueue[BuffersCount];
  Data data[BuffersCount];
  bool readerDone;
};
class SharedMemory {
public:
  SharedMemory();
  ~SharedMemory();

  Data *getFreeBuffer();
  void pushToWriteQueue(Data *data);
  Data *popFromWriteQueue();
  void releaseBuffer(Data *data);
  bool attemptRelease();
  void setReaderDone(const bool value);

private:
  bool isBufferFree() const;
  bool isReaderDone() const;
  void clearResources();

private:
  NamedMutex namedMutex_;
  int id_;
  Buffer *buffer_;
};

#endif