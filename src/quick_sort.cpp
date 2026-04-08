#include "quick_sort.h"

#include <algorithm>

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
