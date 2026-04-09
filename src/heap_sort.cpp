#include "heap_sort.h"
#include "input_gen.h"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

long long g_swaps = 0;

inline void swapCounted(std::vector<int> &A, int i, int j)
{
  if (i == j) {
    return;
  }

  std::swap(A[i], A[j]);
  ++g_swaps;
}

void siftDown(std::vector<int> &A, int heapSize, int root)
{
  while (true) {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if (left < heapSize && A[left] > A[largest]) {
      largest = left;
    }

    if (right < heapSize && A[right] > A[largest]) {
      largest = right;
    }

    if (largest == root) {
      return;
    }

    swapCounted(A, root, largest);
    root = largest;
  }
}

static double timeSortFunction(void (*sortFunc)(std::vector<int> &),
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

long long averageHeapSwaps(const std::vector<int> &base, int trials)
{
  long long totalSwaps = 0;

  for (int t = 0; t < trials; ++t) {
    std::vector<int> data = base;
    resetHeapSortSwapCount();
    heapSort(data);
    totalSwaps += getHeapSortSwapCount();
  }

  return totalSwaps / trials;
}

void heapSort(std::vector<int> &A)
{
  int n = static_cast<int>(A.size());
  if (n <= 1) {
    return;
  }

  for (int i = n / 2 - 1; i >= 0; --i) {
    siftDown(A, n, i);
  }

  for (int end = n - 1; end > 0; --end) {
    swapCounted(A, 0, end);
    siftDown(A, end, 0);
  }
}

long long getHeapSortSwapCount() { return g_swaps; }

void resetHeapSortSwapCount() { g_swaps = 0; }

void heapSortAnalysis(const std::vector<int> &sizes, int trials)
{
  std::cout << "Heap Sort Analysis (in-place max-heap with sift-down)"
            << std::endl;
  std::cout << "Trials per size: " << trials << std::endl;
  std::cout << std::endl;

  std::cout << std::left << std::setw(8) << "n" << std::setw(14) << "rand(us)"
            << std::setw(14) << "equal(us)" << std::setw(18) << "rand_swaps"
            << std::setw(18) << "equal_swaps" << std::setw(16)
            << "rand/(nlog2n)" << std::setw(16) << "equal/(nlog2n)"
            << std::endl;

  for (int n : sizes) {
    std::vector<int> randomInput = generateUnique(n, 43 + n);
    std::vector<int> equalInput(n, 7);

    double randomUs = timeSortFunction(heapSort, randomInput, trials);
    double equalUs = timeSortFunction(heapSort, equalInput, trials);

    long long randomSwaps = averageHeapSwaps(randomInput, trials);
    long long equalSwaps = averageHeapSwaps(equalInput, trials);

    double nLogN = n * std::log2(n);
    double randomNormalized = randomSwaps / nLogN;
    double equalNormalized = equalSwaps / nLogN;

    std::cout << std::left << std::setw(8) << n << std::setw(14) << std::fixed
              << std::setprecision(3) << randomUs << std::setw(14)
              << std::setprecision(3) << equalUs << std::setw(18) << randomSwaps
              << std::setw(18) << equalSwaps << std::setw(16)
              << std::setprecision(6) << randomNormalized << std::setw(16)
              << std::setprecision(6) << equalNormalized << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Swap ratio stays bounded, consistent with O(n log n) behavior."
            << std::endl;
}
