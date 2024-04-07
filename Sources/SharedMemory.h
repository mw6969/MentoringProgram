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

  void setData(const short index, const char data[], const size_t size);
  std::string getData(const short index) const;
  short getFreeBufferIndex();
  short getReadyForWriteBufferIndex();
  bool isBufferFree() const;
  void setReaderDone(const bool value);
  bool isReaderDone() const;
  void destroy();

private:
  void setReadyForWriteBufferIndex(const short index);
  void setFirstFreeBufferIndex(const short index);

private:
  Buffer *buffer_;
  int id_;
};

#endif