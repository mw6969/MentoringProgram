#ifndef _PARENTPROCESS_
#define _PARENTPROCESS_

class ParentProcess {
public:
  ParentProcess();

private:
  static void handleChildSignal(int signal);
};

#endif