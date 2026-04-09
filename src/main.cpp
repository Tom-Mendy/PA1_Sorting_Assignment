#include "merge_sort.h"
#include "quick_sort.h"

#include <iostream>
#include <string>

int main()
{
  std::vector<int> sizes = {1000, 2000, 4000, 8000, 16000, 32000};
  int trials = 3;
  quickSortAnalysis(sizes, trials);
  std::cout << std::string(90, '-') << std::endl;
  mergeSortAnalysis(sizes, trials);
  return 0;
}
