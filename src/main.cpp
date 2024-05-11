#include <sys/wait.h>

#include "CustomException.h"
#include "Logger.h"
#include "Tool.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    Logger log(
        "Invalid input args. Use format: ./app \"source\" \"destination\"");
    return -1;
  }

  std::set_terminate(CustomException::handler);

  if (pid_t pid{fork()}; pid == -1) {
    throw CustomException("Failed to fork");
  } else if (pid > 0) { // parent
    try {
      Tool tool;
      tool.reader(argv[1]);
    } catch (...) {
      if (kill(pid, SIGTERM) == -1) {
        Logger log("Can not send SIGTERM to child process");
        return 1;
      } else {
        waitpid(pid, nullptr, 0);
      }
      if (auto eptr = std::current_exception(); eptr) {
        std::rethrow_exception(eptr);
      }
    }
  } else { // child
    try {
      Tool tool;
      tool.writer(argv[2]);
    } catch (...) {
      kill(getppid(), SIGTERM);
      if (auto eptr = std::current_exception(); eptr) {
        std::rethrow_exception(eptr);
      }
    }
  }

  return 0;
}