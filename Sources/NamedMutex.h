#ifndef _NAMEDMUTEX_
#define _NAMEDMUTEX_

#include <atomic>
class NamedMutex {
public:
  NamedMutex();
  ~NamedMutex();

  void lock();
  void unlock();
  static const char *getName();

private:
  void clearResources();

private:
  int shmFd_;
  std::atomic_flag *mutex_;
  static const char *name_;
};

#endif