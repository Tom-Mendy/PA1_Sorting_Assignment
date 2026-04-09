#include "radix_sort.h"
#include "counting_sort.h"
#include "heap_sort.h"
#include "input_gen.h"
#include "merge_sort.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr int BASE = 10;

int computeDigitPasses(int maxVal)
{
  if (maxVal < 0) {
    throw std::invalid_argument("maxVal must be non-negative");
  }

  int passes = 1;
  while (maxVal >= BASE) {
    maxVal /= BASE;
    ++passes;
  }
  return passes;
}

void stableCountingPassByDigit(std::vector<int> &A, int exp)
{
  std::vector<int> count(BASE, 0);

  for (int value : A) {
    int digit = (value / exp) % BASE;
    ++count[digit];
  }

  for (int i = 1; i < BASE; ++i) {
    count[i] += count[i - 1];
  }

  std::vector<int> output(A.size());

  for (int i = static_cast<int>(A.size()) - 1; i >= 0; --i) {
    int digit = (A[i] / exp) % BASE;
    int placeIndex = count[digit] - 1;
    output[placeIndex] = A[i];
    --count[digit];
  }

  A = output;
}

static double
timeRadixSort(const std::vector<int> &base, int maxVal, int trials)
{
  long long totalUs = 0;

  for (int t = 0; t < trials; ++t) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> sorted = radixSortLSDBase10(base, maxVal);
    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(sorted.begin(), sorted.end())) {
      throw std::runtime_error("radixSortLSDBase10 produced unsorted output");
    }

    long long runTime =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    totalUs += runTime;
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

static double
timeCountingSort(const std::vector<int> &base, int maxVal, int trials)
{
  long long totalUs = 0;

  for (int t = 0; t < trials; ++t) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> sorted = countingSort(base, maxVal);
    auto end = std::chrono::high_resolution_clock::now();

    if (!std::is_sorted(sorted.begin(), sorted.end())) {
      throw std::runtime_error("countingSort produced unsorted output");
    }

    long long runTime =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    totalUs += runTime;
  }

  return static_cast<double>(totalUs) / trials;
}

} // namespace

std::vector<int> radixSortLSDBase10(const std::vector<int> &input, int maxVal)
{
  if (maxVal < 0) {
    throw std::invalid_argument("maxVal must be non-negative");
  }

  std::vector<int> output = input;

  for (int value : output) {
    if (value < 0 || value > maxVal) {
      throw std::out_of_range("input value outside [0, maxVal]");
    }
  }

  int passes = computeDigitPasses(maxVal);
  int exp = 1;

  for (int pass = 0; pass < passes; ++pass) {
    stableCountingPassByDigit(output, exp);
    exp *= BASE;
  }

  return output;
}

void radixSortAnalysis(const std::vector<int> &sizes, int trials)
{
  constexpr int maxVal = 99999;
  int passes = computeDigitPasses(maxVal);

  std::cout << "Radix Sort Analysis (LSD base-10, stable per-digit counting)"
            << std::endl;
  std::cout << "Trials per size: " << trials << std::endl;
  std::cout << "maxVal: " << maxVal << ", base: " << BASE
            << ", computed passes d: " << passes << std::endl;
  std::cout << std::endl;

  std::cout << std::left << std::setw(8) << "n" << std::setw(14) << "radix(us)"
            << std::setw(14) << "merge(us)" << std::setw(14) << "heap(us)"
            << std::setw(16) << "counting(us)" << std::setw(18)
            << "radix/(d*(n+b))" << std::endl;

  for (int n : sizes) {
    std::vector<int> nonUnique = generateNonUnique(n, maxVal, 777 + n);

    double radixUs = timeRadixSort(nonUnique, maxVal, trials);
    double mergeUs = timeInPlaceSort(mergeSort, nonUnique, trials);
    double heapUs = timeInPlaceSort(heapSort, nonUnique, trials);
    double countingUs = timeCountingSort(nonUnique, maxVal, trials);

    double normalized = radixUs / (passes * (n + BASE));

    std::cout << std::left << std::setw(8) << n << std::setw(14) << std::fixed
              << std::setprecision(3) << radixUs << std::setw(14)
              << std::setprecision(3) << mergeUs << std::setw(14)
              << std::setprecision(3) << heapUs << std::setw(16)
              << std::setprecision(3) << countingUs << std::setw(18)
              << std::setprecision(6) << normalized << std::endl;
  }

  std::cout << std::endl;
  std::cout << "All radix trials verified sorted output with std::is_sorted."
            << std::endl;
}
