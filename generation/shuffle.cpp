#pragma once
#include <algorithm>
#include <vector>

class Shuffler {
 public:
  static std::vector<size_t> GetShuffle(size_t list_size, size_t seed) {
    const std::vector<size_t>& modules = kPrimes;
    std::vector<size_t> shuffle;
    bool found = false;
    size_t module_idx = list_size - 1;

    while ((!found) && (module_idx < modules.size())) {
      std::vector<std::pair<size_t, size_t>> prioritized;
      for (size_t i = 0; i < list_size; ++i) {
        size_t module = modules[module_idx - i];
        size_t priority = seed % module;
        prioritized.push_back(std::pair(i, priority));
      }
      ++module_idx;

      std::sort(prioritized.begin(), prioritized.end(),
                [](const std::pair<size_t, size_t>& pair1,
                   const std::pair<size_t, size_t>& pair2) {
                  return (pair1.second < pair2.second);
                });

      shuffle.clear();
      for (const auto& pair : prioritized) {
        shuffle.push_back(pair.first);
      }
      found = CheckShuffle(shuffle);
    }

    if (!found) {
      throw "failed to generate a valid shuffle";
    }
    return shuffle;
  }

 private:
  static const std::vector<size_t> kPrimes;

  static bool CheckShuffle(const std::vector<size_t>& shuffle) {
    size_t size = shuffle.size();
    std::vector<size_t> quantities(size, 0);
    for (size_t i = 0; i < size; ++i) {
      size_t number = shuffle[i];
      if ((number == i) || (number >= size)) {
        return false;
      }
      ++quantities[shuffle[i]];
    }
    for (const auto& quan : quantities) {
      if (quan != 1) {
        return false;
      }
    }
    return true;
  }
};

const std::vector<size_t> Shuffler::kPrimes{
    107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
    181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
    263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347,
    349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419};

template <typename T>
std::vector<std::pair<T, T>> PairShuffled(const std::vector<T>& items, size_t shuffle_seed) {
  Shuffler shuffler;
  size_t quan = items.size();
  auto shuffle_order = shuffler.GetShuffle(quan, shuffle_seed);

  std::vector<std::pair<T, T>> result;
  for (size_t i = 0; i < quan; ++i) {
    std::pair pair(items[i], items[shuffle_order[i]]);
    result.push_back(pair);
  }

  return result;
}