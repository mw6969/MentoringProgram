#include "CustomException.h"
#include "Logger.h"

const char *CustomException::what() const noexcept { return message_.c_str(); }

void CustomException::handler() {
  try {
    if (auto eptr = std::current_exception(); eptr) {
      std::rethrow_exception(eptr);
    }
  } catch (const CustomException &e) {
    Logger log(e.what());
  } catch (...) {
    Logger log("Unknown error occurred while handling custom exception");
  }
  std::abort();
}