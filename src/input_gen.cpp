#include "input_gen.h"

#include <algorithm>
#include <random>
#include <stdexcept>

std::vector<int> generateUnique(int n, int seed)
{
  if (n < 0) {
    throw std::invalid_argument("n must be non-negative");
  }

  std::vector<int> pool;

  for (int value = 1; value <= 10 * n; ++value) {
    pool.push_back(value);
  }

  std::mt19937 rng(seed);
  std::shuffle(pool.begin(), pool.end(), rng);

  pool.resize(n);
  return pool;
}

std::vector<int> generateNonUnique(int n, int maxVal, int seed)
{
  if (n < 0) {
    throw std::invalid_argument("n must be non-negative");
  }
  if (maxVal < 0) {
    throw std::invalid_argument("maxVal must be non-negative");
  }

  std::vector<int> out;

  std::mt19937 rng(seed);
  std::uniform_int_distribution<int> dist(0, maxVal);

  for (int i = 0; i < n; ++i) {
    out.push_back(dist(rng));
  }

  return out;
}
