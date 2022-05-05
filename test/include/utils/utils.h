#ifndef MINISQL_UTILS_H
#define MINISQL_UTILS_H

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include "buffer/buffer_pool_manager.h"
#include "storage/disk_manager.h"

template<typename T>
void ShuffleArray(std::vector<T> &array) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::shuffle(array.begin(), array.end(), rng);
}

class RandomUtils {
public:
  static void RandomString(char *buf, size_t len) {
    std::random_device r;
    std::default_random_engine rng(r());
    std::uniform_int_distribution<char> uniform_dist(0);
    for (size_t i = 0; i < len; i++) {
      buf[i] = uniform_dist(rng);
    }
  }

  static int32_t RandomInt(int32_t low, int32_t high) {
    std::random_device r;
    std::default_random_engine rng(r());
    std::uniform_int_distribution<int32_t> uniform_dist(low, high);
    return uniform_dist(rng);
  }

  static float_t RandomFloat(float_t low, float_t high) {
    std::random_device r;
    std::default_random_engine rng(r());
    std::uniform_real_distribution<float_t> uniform_dist(low, high);
    return uniform_dist(rng);
  }

};

#endif //MINISQL_UTILS_H
