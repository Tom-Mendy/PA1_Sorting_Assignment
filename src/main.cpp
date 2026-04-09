#include "counting_sort.h"
#include "heap_sort.h"
#include "input_gen.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include "radix_sort.h"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

long long medianOfThree(std::vector<long long> values)
{
  if (values.size() != 3U) {
    throw std::invalid_argument("medianOfThree requires exactly 3 values");
  }

  std::sort(values.begin(), values.end());
  return values[1];
}

long long benchmarkInPlaceSort(void (*sortFunc)(std::vector<int> &),
                               const std::vector<int> &base,
                               int trials)
{
  std::vector<long long> runTimes;
  runTimes.reserve(trials);

  for (int t = 0; t < trials; ++t) {
    std::vector<int> data = base;

    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = std::chrono::high_resolution_clock::now();

    long long us =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    runTimes.push_back(us);
  }

  return medianOfThree(runTimes);
}

long long
benchmarkCountingSort(const std::vector<int> &base, int maxVal, int trials)
{
  std::vector<long long> runTimes;
  runTimes.reserve(trials);

  for (int t = 0; t < trials; ++t) {
    std::vector<int> data = base;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> sorted = countingSort(data, maxVal);
    auto end = std::chrono::high_resolution_clock::now();

    if (sorted.size() != data.size()) {
      throw std::runtime_error("countingSort output size mismatch");
    }

    long long us =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    runTimes.push_back(us);
  }

  return medianOfThree(runTimes);
}

long long
benchmarkRadixSort(const std::vector<int> &base, int maxVal, int trials)
{
  std::vector<long long> runTimes;
  runTimes.reserve(trials);

  for (int t = 0; t < trials; ++t) {
    std::vector<int> data = base;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> sorted = radixSortLSDBase10(data, maxVal);
    auto end = std::chrono::high_resolution_clock::now();

    if (sorted.size() != data.size()) {
      throw std::runtime_error("radixSort output size mismatch");
    }

    long long us =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    runTimes.push_back(us);
  }

  return medianOfThree(runTimes);
}

void printHeader()
{
  std::cout << std::left << std::setw(14) << "Algorithm" << std::setw(12)
            << "n=1,000" << std::setw(12) << "n=5,000" << std::setw(13)
            << "n=10,000" << std::setw(13) << "n=50,000" << std::setw(13)
            << "n=100,000" << std::setw(13) << "n=500,000" << '\n';
  std::cout << std::string(76, '-') << '\n';
}

void printComparisonRow(const std::string &name,
                        long long t1000,
                        long long t5000,
                        long long t10000,
                        long long t50000)
{
  std::cout << std::left << std::setw(14) << name << std::setw(12) << t1000
            << std::setw(12) << t5000 << std::setw(13) << t10000
            << std::setw(13) << t50000 << std::setw(13) << "-" << std::setw(13)
            << "-" << '\n';
}

void printNonComparisonRow(const std::string &name,
                           long long t10000,
                           long long t50000,
                           long long t100000,
                           long long t500000)
{
  std::cout << std::left << std::setw(14) << name << std::setw(12) << "-"
            << std::setw(12) << "-" << std::setw(13) << t10000 << std::setw(13)
            << t50000 << std::setw(13) << t100000 << std::setw(13) << t500000
            << '\n';
}

} // namespace

int main()
{
  constexpr int trials = 3;

  const std::vector<int> comparisonSizes = {1000, 5000, 10000, 50000};
  const std::vector<int> nonComparisonSizes = {10000, 50000, 100000, 500000};

  constexpr int nonComparisonMaxVal = 100000;
  constexpr int seedBase = 2026;

  std::vector<std::vector<int>> comparisonInputs;
  comparisonInputs.reserve(comparisonSizes.size());

  for (int n : comparisonSizes) {
    comparisonInputs.push_back(generateUnique(n, seedBase + n));
  }

  std::vector<std::vector<int>> nonComparisonInputs;
  nonComparisonInputs.reserve(nonComparisonSizes.size());

  for (int n : nonComparisonSizes) {
    nonComparisonInputs.push_back(
        generateNonUnique(n, nonComparisonMaxVal, seedBase + n));
  }

  long long quick1000 =
      benchmarkInPlaceSort(quickSort, comparisonInputs[0], trials);
  long long quick5000 =
      benchmarkInPlaceSort(quickSort, comparisonInputs[1], trials);
  long long quick10000 =
      benchmarkInPlaceSort(quickSort, comparisonInputs[2], trials);
  long long quick50000 =
      benchmarkInPlaceSort(quickSort, comparisonInputs[3], trials);

  long long merge1000 =
      benchmarkInPlaceSort(mergeSort, comparisonInputs[0], trials);
  long long merge5000 =
      benchmarkInPlaceSort(mergeSort, comparisonInputs[1], trials);
  long long merge10000 =
      benchmarkInPlaceSort(mergeSort, comparisonInputs[2], trials);
  long long merge50000 =
      benchmarkInPlaceSort(mergeSort, comparisonInputs[3], trials);

  long long heap1000 =
      benchmarkInPlaceSort(heapSort, comparisonInputs[0], trials);
  long long heap5000 =
      benchmarkInPlaceSort(heapSort, comparisonInputs[1], trials);
  long long heap10000 =
      benchmarkInPlaceSort(heapSort, comparisonInputs[2], trials);
  long long heap50000 =
      benchmarkInPlaceSort(heapSort, comparisonInputs[3], trials);

  long long counting10000 = benchmarkCountingSort(
      nonComparisonInputs[0], nonComparisonMaxVal, trials);
  long long counting50000 = benchmarkCountingSort(
      nonComparisonInputs[1], nonComparisonMaxVal, trials);
  long long counting100000 = benchmarkCountingSort(
      nonComparisonInputs[2], nonComparisonMaxVal, trials);
  long long counting500000 = benchmarkCountingSort(
      nonComparisonInputs[3], nonComparisonMaxVal, trials);

  long long radix10000 =
      benchmarkRadixSort(nonComparisonInputs[0], nonComparisonMaxVal, trials);
  long long radix50000 =
      benchmarkRadixSort(nonComparisonInputs[1], nonComparisonMaxVal, trials);
  long long radix100000 =
      benchmarkRadixSort(nonComparisonInputs[2], nonComparisonMaxVal, trials);
  long long radix500000 =
      benchmarkRadixSort(nonComparisonInputs[3], nonComparisonMaxVal, trials);

  std::cout << "Benchmark results (median of 3 runs, microseconds)" << '\n';
  std::cout << "comparison input: unique arrays" << '\n';
  std::cout << "non-comparison input: non-unique arrays, maxVal="
            << nonComparisonMaxVal << '\n';
  std::cout << "seed base: " << seedBase << '\n' << '\n';

  printHeader();
  printComparisonRow(
      "Quick Sort", quick1000, quick5000, quick10000, quick50000);
  printComparisonRow(
      "Merge Sort", merge1000, merge5000, merge10000, merge50000);
  printComparisonRow("Heap Sort", heap1000, heap5000, heap10000, heap50000);
  printNonComparisonRow("Counting Sort",
                        counting10000,
                        counting50000,
                        counting100000,
                        counting500000);
  printNonComparisonRow(
      "Radix Sort", radix10000, radix50000, radix100000, radix500000);

  return 0;
}
