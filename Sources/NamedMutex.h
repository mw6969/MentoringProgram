#ifndef _NAMEDMUTEX_
#define _NAMEDMUTEX_

#include <atomic>
#include <sys/mman.h>

class NamedMutex {
public:
  NamedMutex(const char *mutexName);
  ~NamedMutex();

  void lock();
  void unlock();

private:
  std::atomic_flag *mutex_;
  int shmFd_;
};

#endif