#include "Application.h"

#include <iostream>
#include <memory>

/// @brief Usage: ./app <server|client> <file1> <file2> ...
/// Run server: ./app server
/// Run client: ./app client input.txt bin.jpg
int main(int argc, char *argv[]) {
  if (argc < 1) {
    throw std::runtime_error("Invalid input args");
    return EXIT_FAILURE;
  }

  static std::unique_ptr<Application> app;
  std::set_terminate([]() {
    app.reset();
    std::cerr << "Unhandled exception\n";
    std::abort();
  });

  try {
    app = std::make_unique<Application>(argc, argv);
    app->run();
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << "\n";
  }

  return EXIT_SUCCESS;
}