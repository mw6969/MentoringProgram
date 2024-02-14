#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <stddef.h>
#include <string>

constexpr short BuffersCount = 5;
constexpr short DataLength = 100;
struct Buffer {
  short freeIndexes[BuffersCount];
  short readyForWriteIndexes[BuffersCount];
  char data[BuffersCount][DataLength];
};
class SharedMemory {
public:
  SharedMemory();
  ~SharedMemory() = default;

  void setData(const short index, const std::string &data);
  std::string getData(const short index)const;
  short getFreeBufferIndex() const;
  void setFirstFreeBufferIndex(const short index);
  void setReadyForWriteBufferIndex(const short index);
  short getFirstReadyForWriteBufferIndex() const;
  void shiftFirstReadyForWriteBufferIndex();
  bool checkFreeBufferIndex();
  void destroy();

private:
  Buffer *buffer_;
  int id_;
};

#endif