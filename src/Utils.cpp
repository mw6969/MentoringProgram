#include "Utils.h"

#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <unordered_set>

void Utils::PrintMeasureTime(std::function<void(std::vector<double> &)> func,
                             const int value, const std::string &method) {
  std::vector<double> doubles = GenerateUniqueDoubles(value, 1.0, value);
  auto start = std::chrono::high_resolution_clock::now();
  func(doubles);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << method << " time: " << duration.count() << " microseconds"
            << std::endl;
}

void Utils::Quicksort(std::vector<double> &arr, const int low, const int high) {
  if (low < high) {
    int pivot = Partition(arr, low, high);
    Quicksort(arr, low, pivot - 1);
    Quicksort(arr, pivot + 1, high);
  }
}

void Utils::MultiThreadedQuicksort(std::vector<double> &arr, int low, int high,
                                   int depth) {
  if (low < high) {
    int pivot = Partition(arr, low, high);

    int maxDepth = std::thread::hardware_concurrency();
    if (depth < maxDepth) {
      auto handle =
          std::async(std::launch::async, &Utils::MultiThreadedQuicksort,
                     std::ref(arr), low, pivot - 1, depth + 1);
      MultiThreadedQuicksort(arr, pivot + 1, high, depth + 1);
      handle.get();
    } else {
      MultiThreadedQuicksort(arr, low, pivot - 1, depth);
      MultiThreadedQuicksort(arr, pivot + 1, high, depth);
    }
  }
}

int Utils::Partition(std::vector<double> &arr, const int low, const int high) {
  double pivot = arr[high];
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++) {
    if (arr[j] < pivot) {
      i++;
      std::swap(arr[i], arr[j]);
    }
  }
  std::swap(arr[i + 1], arr[high]);
  return (i + 1);
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