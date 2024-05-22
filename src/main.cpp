#include <iostream>
#include <memory>

#include "Application.h"
#include "CustomException.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    throw CustomException(
        "Invalid input args. Use format: ./app \"source\" \"destination\"");
    return -1;
  }

  static std::unique_ptr<Application> app;
  std::set_terminate([]() {
    app.reset();
    std::cerr << "Unhandled exception\n";
    std::abort();
  });

  try {
    app = std::make_unique<Application>(argv[1], argv[2]);
  } catch (const CustomException &ex) {
    std::cerr << ex.what() << "\n";
  }

  return EXIT_SUCCESS;
}