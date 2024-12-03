#include "Utils.h"

#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <unordered_set>

void Utils::Quicksort(std::vector<double> &arr, const int low, const int high) {
  int i = low;
  int j = high;
  Partition(arr, GetPivot(arr, low, high), i, j);
  if (low < j) {
    Quicksort(arr, low, j);
  }
  if (i < high) {
    Quicksort(arr, i, high);
  }
}

void Utils::MultiThreadedQuicksort(std::vector<double> &arr, int low, int high,
                                   std::counting_semaphore<64> &semaphore,
                                   int syncSize) {
  int i = low;
  int j = high;
  Partition(arr, GetPivot(arr, low, high), i, j);

  std::future<void> lowFuture;
  std::future<void> highFuture;

  if (low < j) {
    if (j - low >= syncSize && semaphore.try_acquire()) {
      lowFuture = std::async(
          std::launch::async, [&arr, low, j, &semaphore, syncSize]() {
            MultiThreadedQuicksort(arr, low, j, semaphore, syncSize);
          });
      semaphore.release();
    } else {
      MultiThreadedQuicksort(arr, low, j, semaphore, syncSize);
    }
  }
  if (i < high) {
    if (high - i >= syncSize && semaphore.try_acquire()) {
      highFuture = std::async(
          std::launch::async, [&arr, i, high, &semaphore, syncSize]() {
            MultiThreadedQuicksort(arr, i, high, semaphore, syncSize);
          });
      semaphore.release();
    } else {
      MultiThreadedQuicksort(arr, i, high, semaphore, syncSize);
    }
  }
  if (lowFuture.valid()) {
    lowFuture.wait();
  }
  if (highFuture.valid()) {
    highFuture.wait();
  }
}

void Utils::PrintMeasureTime(std::function<void(std::vector<double> &)> func,
                             const int value, const std::string &method,
                             int iterations) {
  std::uint64_t totalUnsortedDuration = 0;
  std::uint64_t totalSortedDuration = 0;
  for (int i = 0; i < iterations; i++) {
    auto doubles = GenerateUniqueDoubles(value, 1.0, value);
    for (int i = 0; i < 2; i++) {
      auto start = std::chrono::high_resolution_clock::now();
      func(doubles);
      auto stop = std::chrono::high_resolution_clock::now();
      (i == 0 ? totalUnsortedDuration : totalSortedDuration) +=
          std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
              .count();
    }
  }
  std::cout << "Unsorted " << method << " time for " << value
            << " items: " << totalUnsortedDuration / iterations
            << " microseconds" << std::endl;
  std::cout << "Sorted " << method << " time for " << value
            << " items: " << totalSortedDuration / iterations << " microseconds"
            << std::endl;
}

double Utils::GetPivot(std::vector<double> &arr, int low, int high) {
  if (arr[low] < arr[(low + high) / 2]) {
    if (arr[(low + high) / 2] < arr[high]) {
      return arr[(low + high) / 2];
    } else if (arr[low] < arr[high]) {
      return arr[high];
    } else {
      return arr[low];
    }
  } else {
    if (arr[low] < arr[high]) {
      return arr[low];
    } else if (arr[(low + high) / 2] < arr[high]) {
      return arr[high];
    } else {
      return arr[(low + high) / 2];
    }
  }
}

void Utils::Partition(std::vector<double> &arr, double pivot, int &i, int &j) {
  while (i <= j) {
    while (arr[i] < pivot) {
      i++;
    }
    while (arr[j] > pivot) {
      j--;
    }
    if (i <= j) {
      std::swap(arr[i], arr[j]);
      i++;
      j--;
    }
  }
}

std::vector<double> Utils::GenerateUniqueDoubles(const size_t numElements,
                                                 const double min,
                                                 const double max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(min, max);
  std::unordered_set<double> uniqueNumbers;
  while (uniqueNumbers.size() < numElements) {
    uniqueNumbers.insert(dis(gen));
  }
  return std::vector<double>(uniqueNumbers.begin(), uniqueNumbers.end());
}