#include "input_gen.h"
#include "quick_sort.h"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

static std::vector<int> makeAllEqual(int n, int value)
{
  return std::vector<int>(n, value);
}

static double timeSortFunction(void (*sortFunc)(std::vector<int> &),
                               const std::vector<int> &base,
                               int trials)
{
  long long totalMs = 0.0;

  for (int t = 0; t < trials; ++t) {
    std::vector<int> data = base;

    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = std::chrono::high_resolution_clock::now();
    long long runTime =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    totalMs += runTime;
  }

  return totalMs / trials;
}

static void quickSortAnalysis(std::vector<int> sizes, int trials)
{
  std::cout
      << "Quick Sort Analysis (median-of-three + Lomuto + insertion cutoff<=10)"
      << std::endl;
  std::cout << "Trials per size: " << trials << std::endl;
  std::cout << std::endl;

  std::cout << std::left << std::setw(8) << "n" << std::setw(14) << "random(ms)"
            << std::setw(14) << "equal(ms)" << std::setw(16) << "rand/(nlog2n)"
            << std::setw(14) << "equal/(n^2)" << std::endl;

  for (int n : sizes) {
    std::vector<int> randomInput = generateUnique(n, 43 + n);
    std::vector<int> equalInput = makeAllEqual(n, 7);

    double randomMs = timeSortFunction(quickSort, randomInput, trials);
    double equalMs = timeSortFunction(quickSort, equalInput, trials);

    double nLogN = n * std::log2(n);
    double n2 = n * n;

    double randomNormalized = randomMs / nLogN;
    double equalNormalized = equalMs / n2;

    std::cout << std::left << std::setw(8) << n << std::setw(14) << std::fixed
              << std::setprecision(3) << randomMs << std::setw(14) << std::fixed
              << std::setprecision(3) << equalMs << std::setw(16) << std::fixed
              << std::setprecision(9) << randomNormalized << std::setw(14)
              << std::fixed << std::setprecision(12) << equalNormalized
              << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Worst-case style input used: all elements equal." << std::endl;
  std::cout << "With Lomuto partition and condition A[j] < pivot, this creates "
               "very unbalanced splits."
            << std::endl;
}

int main()
{
  std::vector<int> sizes = {1000, 2000, 4000, 8000, 16000, 32000};
  int trials = 3;
  quickSortAnalysis(sizes, trials);
  return 0;
}
