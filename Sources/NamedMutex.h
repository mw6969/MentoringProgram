#ifndef _NAMEDMUTEX_
#define _NAMEDMUTEX_

#include <atomic>
#include <sys/mman.h>

class NamedMutex {
public:
  NamedMutex();
  ~NamedMutex();

  void lock();
  void unlock();
  static const char *getName();

private:
  static const char *name_;
  std::atomic_flag *mutex_;
  int shmFd_;
};

#endif