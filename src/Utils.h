#ifndef _UTILS_
#define _UTILS_

#include <functional>
#include <string>
#include <vector>

class Utils {
public:
  static void Quicksort(std::vector<double> &arr, const int low,
                        const int high);

  static void MultiThreadedQuicksort(std::vector<double> &arr, const int low,
                                     const int high, int depth = 0,
                                     int syncSize = 10000);

  static void PrintMeasureTime(std::function<void(std::vector<double> &)> func,
                               const int value, const std::string &method,
                               int iterations = 100);

private:
  static double GetPivot(std::vector<double> &arr, int low, int high);
  static void Partition(std::vector<double> &arr, double pivot, int &i, int &j);
  static std::vector<double> GenerateUniqueDoubles(size_t numElements,
                                                   double min, double max);
};

#endif