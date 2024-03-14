#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <string>

constexpr short BuffersCount = 5;
constexpr short DataLength = 100;
struct Buffer {
  short freeIndexes[BuffersCount];
  short readyForWriteIndexes[BuffersCount];
  char data[BuffersCount][DataLength];
  bool readerDone;
};
class SharedMemory {
public:
  SharedMemory();
  ~SharedMemory() = default;

  void setData(const short index, const std::string &data);
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