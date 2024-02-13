#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <stddef.h>
#include <string>

constexpr int stringsMaxLength = 100;

struct String {
  int id;
  char data[stringsMaxLength];
};

struct Collection {
  size_t size;
  size_t first;
  String *strings;
};
class SharedMemory {
public:
  SharedMemory();
  ~SharedMemory() = default;

  void push(const std::string &data);
  void popFront();
  std::string front();
  void destroy();
  bool empty();

private:
  Collection *collection_;
  int collectionId_;
  int stringId_;
};

#endif