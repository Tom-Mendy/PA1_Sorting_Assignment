#include "merge_sort.h"
#include "input_gen.h"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
namespace {

long long g_comparisons = 0;

void mergeRange(
    std::vector<int> &A, std::vector<int> &aux, int left, int mid, int right)
{
  int i = left;
  int j = mid + 1;
  int k = left;

  while (i <= mid && j <= right) {
    ++g_comparisons;
    if (A[i] <= A[j]) {
      aux[k++] = A[i++];
    } else {
      aux[k++] = A[j++];
    }
  }

  while (i <= mid) {
    aux[k++] = A[i++];
  }

  while (j <= right) {
    aux[k++] = A[j++];
  }

  for (int idx = left; idx <= right; ++idx) {
    A[idx] = aux[idx];
  }
}

void mergeSortImpl(std::vector<int> &A,
                   std::vector<int> &aux,
                   int left,
                   int right)
{
  if (left >= right) {
    return;
  }

  int mid = left + (right - left) / 2;
  mergeSortImpl(A, aux, left, mid);
  mergeSortImpl(A, aux, mid + 1, right);
  mergeRange(A, aux, left, mid, right);
}

} // namespace

void mergeSort(std::vector<int> &A)
{
  if (A.empty()) {
    return;
  }

  std::vector<int> aux(A.size());
  mergeSortImpl(A, aux, 0, static_cast<int>(A.size()) - 1);
}

long long getMergeSortComparisonCount() { return g_comparisons; }

void resetMergeSortComparisonCount() { g_comparisons = 0; }

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

static long long averageMergeComparisons(const std::vector<int> &base,
                                         int trials)
{
  long long totalComparisons = 0;
  for (int t = 0; t < trials; ++t) {
    std::vector<int> data = base;
    resetMergeSortComparisonCount();
    mergeSort(data);
    totalComparisons += getMergeSortComparisonCount();
  }

  return totalComparisons / trials;
}

void mergeSortAnalysis(const std::vector<int> &sizes, int trials)
{
  std::cout << "Merge Sort Analysis (top-down recursive, stable merge)"
            << std::endl;
  std::cout << "Trials per size: " << trials << std::endl;
  std::cout << std::endl;

  std::cout << std::left << std::setw(8) << "n" << std::setw(14) << "rand(us)"
            << std::setw(14) << "equal(us)" << std::setw(18) << "rand_cmp"
            << std::setw(18) << "equal_cmp" << std::setw(16) << "rand/(nlog2n)"
            << std::setw(16) << "equal/(nlog2n)" << std::endl;

  for (int n : sizes) {
    std::vector<int> randomInput = generateUnique(n, 43 + n);
    std::vector<int> equalInput(n, 7);

    double randomUs = timeSortFunction(mergeSort, randomInput, trials);
    double equalUs = timeSortFunction(mergeSort, equalInput, trials);

    long long randomCmp = averageMergeComparisons(randomInput, trials);
    long long equalCmp = averageMergeComparisons(equalInput, trials);

    double nLogN = n * std::log2(n);
    double randomNormalized = randomCmp / nLogN;
    double equalNormalized = equalCmp / nLogN;

    std::cout << std::left << std::setw(8) << n << std::setw(14) << std::fixed
              << std::setprecision(3) << randomUs << std::setw(14)
              << std::setprecision(3) << equalUs << std::setw(18) << randomCmp
              << std::setw(18) << equalCmp << std::setw(16)
              << std::setprecision(6) << randomNormalized << std::setw(16)
              << std::setprecision(6) << equalNormalized << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Comparison ratio stays bounded, consistent with O(n log n)."
            << std::endl;
}