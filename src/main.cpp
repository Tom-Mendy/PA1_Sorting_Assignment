#include "input_gen.h"

#include <iostream>

int main()
{
  std::vector<int> input_unique = generateUnique(10, 43);
  std::cout << "Input Unique: " << std::endl;
  for (const auto &num : input_unique) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
  std::vector<int> input_non_unique = generateNonUnique(10, 1000, 43);
  std::cout << "Input Non-Unique: " << std::endl;
  for (const auto &num : input_non_unique) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
  return 0;
}
