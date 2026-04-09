#include "counting_sort.h"
#include "heap_sort.h"
#include "input_gen.h"
#include "merge_sort.h"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<int> countingSort(const std::vector<int> &input, int maxVal)
{
  if (maxVal < 0) {
    throw std::invalid_argument("maxVal must be non-negative");
  }

  std::vector<int> count(maxVal + 1, 0);

  for (int value : input) {
    if (value < 0 || value > maxVal) {
      throw std::out_of_range("input value outside [0, maxVal]");
    }
    ++count[value];
  }

  for (int i = 1; i <= maxVal; ++i) {
    count[i] += count[i - 1];
  }

  std::vector<int> output(input.size());

  for (int i = static_cast<int>(input.size()) - 1; i >= 0; --i) {
    int value = input[i];
    int placeIndex = count[value] - 1;
    output[placeIndex] = value;
    --count[value];
  }

  return output;
}

static double
timeCountingSort(const std::vector<int> &base, int maxVal, int trials)
{
  long long totalUs = 0;

  for (int t = 0; t < trials; ++t) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> sorted = countingSort(base, maxVal);
    auto end = std::chrono::high_resolution_clock::now();

    long long runTime =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    totalUs += runTime;

    if (sorted.size() != base.size()) {
      throw std::runtime_error("countingSort output size mismatch");
    }
  }

  return static_cast<double>(totalUs) / trials;
}

static double timeInPlaceSort(void (*sortFunc)(std::vector<int> &),
                              const std::vector<int> &base,
                              int trials)
{
  long long totalUs = 0;

  for (int t = 0; t < trials; ++t) {
    std::vector<int> data = base;
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = std::chrono::high_resolution_clock::now();

    long long runTime =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    totalUs += runTime;
  }

  return static_cast<double>(totalUs) / trials;
}

void countingSortAnalysis(const std::vector<int> &sizes, int trials)
{
  constexpr int fixedMaxVal = 1000;

  std::cout << "Counting Sort Analysis (stable count/prefix/place, maxVal=1000)"
            << std::endl;
  std::cout << "Trials per size: " << trials << std::endl;
  std::cout << std::endl;

  std::cout << std::left << std::setw(8) << "n" << std::setw(16)
            << "counting(us)" << std::setw(14) << "merge(us)" << std::setw(14)
            << "heap(us)" << std::setw(16) << "count/(n+k)" << std::endl;

  for (int n : sizes) {
    std::vector<int> nonUnique = generateNonUnique(n, fixedMaxVal, 99 + n);

    double countingUs = timeCountingSort(nonUnique, fixedMaxVal, trials);
    double mergeUs = timeInPlaceSort(mergeSort, nonUnique, trials);
    double heapUs = timeInPlaceSort(heapSort, nonUnique, trials);

    double normalized = countingUs / (n + fixedMaxVal);

    std::cout << std::left << std::setw(8) << n << std::setw(16) << std::fixed
              << std::setprecision(3) << countingUs << std::setw(14)
              << std::setprecision(3) << mergeUs << std::setw(14)
              << std::setprecision(3) << heapUs << std::setw(16)
              << std::setprecision(6) << normalized << std::endl;
  }

  std::cout << std::endl;
  std::cout << "k-sweep at n=32000 (counting sort only)" << std::endl;
  std::cout << std::left << std::setw(12) << "k" << std::setw(16)
            << "counting(us)" << std::setw(18) << "count-bytes" << std::setw(18)
            << "count-MiB" << std::endl;

  const int n = 32000;
  std::vector<int> kValues = {1000, 10000, 100000, 1000000};

  for (int k : kValues) {
    std::vector<int> nonUnique = generateNonUnique(n, k, 1000 + k);
    double countingUs = timeCountingSort(nonUnique, k, trials);
    long long bytes = static_cast<long long>(k + 1) * sizeof(int);
    double mib = static_cast<double>(bytes) / (1024.0 * 1024.0);

    std::cout << std::left << std::setw(12) << k << std::setw(16) << std::fixed
              << std::setprecision(3) << countingUs << std::setw(18) << bytes
              << std::setw(18) << std::setprecision(3) << mib << std::endl;
  }

  std::cout << std::endl;
  std::cout << "As expected for O(n + k), runtime and memory both scale with k."
            << std::endl;
}
