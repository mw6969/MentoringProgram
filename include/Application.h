#ifndef _APPLICATION_
#define _APPLICATION_

#include <string>
#include <vector>

class Application {
public:
  Application(int argc, char *argv[]);
  void run();

private:
  std::string mode_;
  std::vector<std::string> fileNames_;
};

#endif