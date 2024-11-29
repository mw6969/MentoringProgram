
#include "Utils.h"

#include <algorithm>
#include <array>

int main() {
  const std::array values{2, 10, 1000, 1000000};
  for (const auto value : values) {
    Utils::PrintMeasureTime(
        [&](std::vector<double> &vec) {
          Utils::Quicksort(vec, 0, vec.size() - 1);
        },
        value, "Quicksort");

    Utils::PrintMeasureTime(
        [](std::vector<double> &vec) { std::sort(vec.begin(), vec.end()); },
        value, "std::sort");

    Utils::PrintMeasureTime(
        [&](std::vector<double> &vec) {
          Utils::MultiThreadedQuicksort(vec, 0, vec.size() - 1);
        },
        value, "Multithreaded quicksort");
  }

  return 0;
}