#include "Application.h"
#include "CustomException.h"
#include "Logger.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    Logger::getInstance().getStream()
        << "Invalid input args. Use format: ./app \"source\" \"destination\"\n";
    return -1;
  }

  try {
    Application app(argv[1], argv[2]);
  } catch (const CustomException &e) {
    Logger::getInstance().getStream() << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (...) {
    Logger::getInstance().getStream() << "Caught an unknown exception\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}