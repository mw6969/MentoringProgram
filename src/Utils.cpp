#include "Utils.h"

#include <array>
#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <unordered_set>

void Utils::PrintMeasureTime(std::function<void(std::vector<double> &)> func,
                             const int value, const std::string &method,
                             int iterations) {
  std::uint64_t totalDuration = 0;
  for (int i = 0; i < iterations; i++) {
    auto doubles = GenerateUniqueDoubles(value, 1.0, value);
    auto start = std::chrono::high_resolution_clock::now();
    func(doubles);
    auto stop = std::chrono::high_resolution_clock::now();
    totalDuration +=
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
            .count();
  }
  const std::uint64_t averageDuration = totalDuration / iterations;
  std::cout << method << " time: " << averageDuration << " microseconds"
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
    int maxDepth = GetCoresCount("lscpu | grep 'Core(s)' | awk '{print $4}'");
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

int Utils::GetCoresCount(const std::string &cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"),
                                                pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
  return std::atoi(result.c_str());
}