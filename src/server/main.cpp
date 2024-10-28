#include "Logger.h"
#include "Server.h"
#include "Utils.h"

#include <memory>

/// @brief Usage: ./server
/// Put key.txt file with a key of 16 characters to the same directory as
/// executable file
int main(int argc, char *argv[]) {
  static Logger &logger = Logger::getInstance();
  static std::unique_ptr<Server> server;
  std::set_terminate([]() {
    server.reset();
    logger.log("Unhandled exception on server");
    std::abort();
  });

  try {
    server = std::make_unique<Server>(Utils::IOService, Utils::Port);
    Utils::IOService.run();
  } catch (const std::exception &ex) {
    logger.log(ex.what());
  }

  return EXIT_SUCCESS;
}