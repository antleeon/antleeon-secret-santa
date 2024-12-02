#pragma once
#include <algorithm>
#include <string>
#include <vector>

class Hasher {
 public:
  static size_t Hash(const std::string& string) {
    std::vector<size_t> as_numbers;
    for (const auto& symb : string) {
      as_numbers.push_back(symb);
    }
    return Hash(as_numbers);
  }

  static size_t Hash(std::vector<std::string> strings, bool unordered = false) {
    if (unordered) {
      std::sort(strings.begin(), strings.end());
    }

    std::vector<size_t> strs_hashes;
    for (const auto& str : strings) {
      strs_hashes.push_back(Hash(str));
    }

    return Hash(strs_hashes);
  }

  static size_t Hash(std::vector<size_t> numbers, bool unordered = false) {
    if (unordered) {
      std::sort(numbers.begin(), numbers.end());
    }

    size_t module = kBigPrimes.back();
    size_t sum = kMidPrimes.back();

    size_t coeffs_quan = kSmallPrimes.size();
    size_t mid_mods_quan = kMidPrimes.size();

    for (size_t i = 0; i < numbers.size(); ++i) {
      size_t numb = numbers[i];
      size_t coeff = kSmallPrimes[i % coeffs_quan];
      size_t mid_mod = kMidPrimes[i % mid_mods_quan];
      sum = (sum + (numb * (coeff % mid_mod))) % module;
    }

    return sum;
  }

 private:
  static const std::vector<size_t> kSmallPrimes;
  static const std::vector<size_t> kMidPrimes;
  static const std::vector<size_t> kBigPrimes;
};

const std::vector<size_t> Hasher::kSmallPrimes{
    107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
    181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
    263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347,
    349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419};
const std::vector<size_t> Hasher::kMidPrimes{
    11159, 11257, 11351, 11447, 11549, 11677, 11779, 11839, 11939, 12037,
    12113, 12227, 12301, 12409, 12491, 12569, 12647, 12743, 12829, 12923,
    13007, 13109, 13187, 13309, 13411, 13499, 13619, 13697, 13781};
const std::vector<size_t> Hasher::kBigPrimes{
    107099, 107641, 108037, 110063, 110567, 110939, 153487,
    154487, 157219, 198637, 199753, 200689, 323837, 341743,
    376351, 527381, 567659, 604249, 766229, 814889, 945907};