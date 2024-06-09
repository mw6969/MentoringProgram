#ifndef _CHILDPROCESS_
#define _CHILDPROCESS_

class ChildProcess {
public:
  ChildProcess();

private:
  static void handleParentSignal(int signal);
};

#endif