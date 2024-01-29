#ifndef _SHAREDMEMORY_
#define _SHAREDMEMORY_

#include <stddef.h>
#include <string>
struct String {
  int id;
  char data[100];
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

  void push(const char *data);
  void popFront();
  const char *front();
  void destroy();
  bool empty();
  size_t size();

private:
  Collection *collection_;
  int collectionId_;
  int stringId_;
};

#endif