#include "Client.h"
#include "Logger.h"
#include "Utils.h"

#include <memory>

/// @brief Usage: ./client <file1> <file2> ...
/// Put key.txt file with a key of 16 characters to the same directory as
/// executable file
int main(int argc, char *argv[]) {
  static Logger &logger = Logger::getInstance();
  if (argc < 2) {
    logger.log("Invalid input args");
    return EXIT_FAILURE;
  }

  static std::unique_ptr<Client> client;
  std::set_terminate([]() {
    client.reset();
    logger.log("Unhandled exception on client");
    std::abort();
  });

  try {
    client = std::make_unique<Client>(Utils::IOService, Utils::Port);
    std::vector<std::string> fileNames;
    for (int i = 1; i < argc; ++i) {
      fileNames.push_back(argv[i]);
    }
    client->sendFiles(std::move(fileNames));
    logger.log("Files sent");
  } catch (const std::exception &ex) {
    logger.log(ex.what());
  }

  return EXIT_SUCCESS;
}