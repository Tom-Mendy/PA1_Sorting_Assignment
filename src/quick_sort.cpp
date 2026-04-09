#include "quick_sort.h"
#include "input_gen.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

constexpr int INSERTION_THRESHOLD = 10;

static void insertionSortRange(std::vector<int> &A, int left, int right)
{
  for (int i = left + 1; i <= right; ++i) {
    int key = A[i];
    int j = i - 1;

    while (j >= left && A[j] > key) {
      A[j + 1] = A[j];
      --j;
    }

    A[j + 1] = key;
  }
}

static int medianOfThreeIndex(std::vector<int> &A, int left, int right)
{
  int mid = left + (right - left) / 2;

  if (A[left] > A[mid]) {
    std::swap(A[left], A[mid]);
  }
  if (A[left] > A[right]) {
    std::swap(A[left], A[right]);
  }
  if (A[mid] > A[right]) {
    std::swap(A[mid], A[right]);
  }

  return mid;
}

static int partitionLomuto(std::vector<int> &A, int left, int right)
{
  int pivotIndex = medianOfThreeIndex(A, left, right);
  std::swap(A[pivotIndex], A[right]);
  int pivot = A[right];
  int i = left - 1;

  for (int j = left; j < right; ++j) {
    if (A[j] < pivot) {
      ++i;
      std::swap(A[i], A[j]);
    }
  }

  std::swap(A[i + 1], A[right]);
  return i + 1;
}

static void quickSortImpl(std::vector<int> &A, int left, int right)
{
  if (left >= right) {
    return;
  }

  if (right - left + 1 <= INSERTION_THRESHOLD) {
    insertionSortRange(A, left, right);
    return;
  }

  int p = partitionLomuto(A, left, right);
  quickSortImpl(A, left, p - 1);
  quickSortImpl(A, p + 1, right);
}

void quickSort(std::vector<int> &A)
{
  if (A.empty()) {
    return;
  }
  quickSortImpl(A, 0, A.size() - 1);
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

void quickSortAnalysis(const std::vector<int> &sizes, int trials)
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
    std::vector<int> equalInput(n, 7);

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