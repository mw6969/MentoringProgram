
#include "Utils.h"

#include <array>
#include <cstdlib>
#include <semaphore>
#include <thread>

int main() {
  const std::array values{2, 10, 1000, 1000000};
  for (const auto value : values) {
    Utils::PrintMeasureTime(
        [&](std::vector<double> &vec) {
          Utils::Quicksort(vec, 0, vec.size() - 1);
        },
        value, "Quicksort");

    Utils::PrintMeasureTime(
        [&](std::vector<double> &vec) {
          std::qsort(vec.data(), vec.size(), sizeof(double),
                     [](const void *a, const void *b) {
                       double arg1 = *static_cast<const double *>(a);
                       double arg2 = *static_cast<const double *>(b);
                       if (arg1 < arg2)
                         return -1;
                       if (arg1 > arg2)
                         return 1;
                       return 0;
                     });
        },
        value, "std::qsort");

    Utils::PrintMeasureTime(
        [&](std::vector<double> &vec) {
          std::counting_semaphore<64> semaphore(
              std::thread::hardware_concurrency());
          Utils::MultiThreadedQuicksort(vec, 0, vec.size() - 1, semaphore,
                                        10000);
        },
        value, "Multithreaded quicksort");
  }

  return 0;
}