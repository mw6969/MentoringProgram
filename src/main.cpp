#include "Application.h"
#include "CustomException.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    throw CustomException(
        "Invalid input args. Use format: ./app \"source\" \"destination\"");
    return -1;
  }

  Application app(argv[1], argv[2]);

  return EXIT_SUCCESS;
}