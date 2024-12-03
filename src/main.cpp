
#include "Utils.h"

#include <array>
#include <semaphore>
#include <thread>

int main() {
  const std::array values{2, 10, 1000, 1000000};
  for (const auto value : values) {
    auto quicksortVec = Utils::GenerateUniqueDoubles(value, 1.0, value);
    Utils::PrintMeasureTime(
        [&quicksortVec]() {
          Utils::Quicksort(quicksortVec, 0, quicksortVec.size() - 1);
        },
        value, "Unsorted Quicksort");
    Utils::PrintMeasureTime(
        [&quicksortVec]() {
          Utils::Quicksort(quicksortVec, 0, quicksortVec.size() - 1);
        },
        value, "Sorted Quicksort");

    auto stdSortVec = Utils::GenerateUniqueDoubles(value, 1.0, value);
    Utils::PrintMeasureTime(
        [&stdSortVec]() { std::sort(stdSortVec.begin(), stdSortVec.end()); },
        value, "Unsorted std::sort");
    Utils::PrintMeasureTime(
        [&stdSortVec]() { std::sort(stdSortVec.begin(), stdSortVec.end()); },
        value, "Sorted std::sort");

    auto multiQuicksortVec = Utils::GenerateUniqueDoubles(value, 1.0, value);
    Utils::PrintMeasureTime(
        [&multiQuicksortVec]() {
          std::counting_semaphore<64> semaphore(
              std::thread::hardware_concurrency());
          Utils::MultiThreadedQuicksort(multiQuicksortVec, 0,
                                        multiQuicksortVec.size() - 1, semaphore,
                                        10000);
        },
        value, "Unsorted Multithreaded quicksort");
    Utils::PrintMeasureTime(
        [&multiQuicksortVec]() {
          std::counting_semaphore<64> semaphore(
              std::thread::hardware_concurrency());
          Utils::MultiThreadedQuicksort(multiQuicksortVec, 0,
                                        multiQuicksortVec.size() - 1, semaphore,
                                        10000);
        },
        value, "Sorted Multithreaded quicksort");
  }

  return 0;
}