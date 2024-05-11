#ifndef _PARENTPROCESS_
#define _PARENTPROCESS_

#include <string>
#include <sys/wait.h>

class ParentProcess {
public:
  ParentProcess(const std::string &inputFile);

  static void handleSigchld(int sig);

private:
  struct sigaction sa_;
};

#endif