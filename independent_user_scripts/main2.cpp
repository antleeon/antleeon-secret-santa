#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

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

class LatinEncoder {
 public:
  static std::string Encode(const std::string& string, size_t min_len = 0) {
    auto string_filled = FillToLength(string, min_len);
    std::string string_encoded("");
    for (const auto& symb : string_filled) {
      string_encoded += CharToCodeString(symb);
    }
    return string_encoded;
  }

  static std::string Decode(std::string string) {
    std::string string_decoded("");
    while (string.size() > 0) {
      std::string char_code_string =
          (string.size() >= 3) ? string.substr(string.size() - 3, 3) : string;
      string = (string.size() > 3) ? string.substr(0, string.size() - 3)
                                   : std::string("");
      char symbol = CodeStringToChar(char_code_string);
      string_decoded =
          (IsFiller(symbol)) ? string_decoded : (symbol + string_decoded);
    }
    return string_decoded;
  }

  static std::vector<std::string> Encode(const std::vector<std::string>& strings) {
    std::vector<std::string> result;
    size_t longest = FindMaxLength(strings);
    for (const auto& str : strings) {
      result.push_back(Encode(str, longest));
    }
    return result;
  }

  static std::vector<std::string> Decode(const std::vector<std::string>& strings) {
    std::vector<std::string> result;
    for (const auto& str : strings) {
      result.push_back(Decode(str));
    }
    return result;
  }

 private:
  static size_t filler_symbol_idx_;
  static const std::vector<char> kFillerSymbols;

  static char GetFiller() {
    size_t array_size = kFillerSymbols.size();
    filler_symbol_idx_ %= array_size;
    char symbol = kFillerSymbols[filler_symbol_idx_];
    filler_symbol_idx_ = (filler_symbol_idx_ + 1) % array_size;
    return symbol;
  }

  static bool IsFiller(char symb) {
    for (const auto& filler : kFillerSymbols) {
      if (filler == symb) {
        return true;
      }
    }
    return false;
  }

  static std::string FillToLength(std::string string, size_t len) {
    while (string.size() < len) {
      char symb = GetFiller();
      string.push_back(symb);
    }
    return string;
  }

  static std::string CharToCodeString(char symbol) {
    size_t char_code = symbol;
    std::string result = std::to_string(char_code / 100) +
                         std::to_string((char_code / 10) % 10) +
                         std::to_string(char_code % 10);
    return result;
  }

  static char CodeStringToChar(const std::string& string_code) {
    size_t char_code = std::stoi(string_code);
    char symbol = char_code;
    return symbol;
  }

  static size_t FindMaxLength(const std::vector<std::string>& strings) {
    size_t max_len = 0;
    for (const auto& str : strings) {
      max_len = std::max(max_len, str.size());
    }
    return max_len;
  }
};

const std::vector<char> LatinEncoder::kFillerSymbols{
    '.', '/', ',', '<', '>', ':', ';', '=', '?', '@', '[', ']', '^', '{', '}'};
size_t LatinEncoder::filler_symbol_idx_ = 0;

class Formater {
 public:
  using Code = size_t;
  using Name = std::string;
  using Member = std::pair<Code, Name>;
  using Group = std::vector<Member>;

  static std::string Format(const Group& result) {
    std::string string("");
    string.push_back(kOpenGroupBracket);
    for (const auto& member : result) {
      string.push_back(kOpenMemberBracket);
      string.append(std::to_string(member.first));
      string.push_back(kInnerMemberSeparator);
      string.append(member.second);
      string.push_back(kCloseMemberBracket);
    }
    string.push_back(kCloseGroupBracket);
    return string;
  }

  static Group DeFormat(const std::string& string) {
    Group result;

    size_t string_len = string.size();
    size_t group_from = string.find_first_of(kOpenGroupBracket);
    size_t group_to = string.find_first_of(kCloseGroupBracket);
    auto group_formated =
        string.substr(group_from + 1, group_to - group_from - 1);

    size_t member_from = group_formated.find_first_of(kOpenMemberBracket);
    size_t member_to = group_formated.find_first_of(kCloseMemberBracket);
    while (member_from != std::string::npos) {
      auto member_formated =
          group_formated.substr(member_from + 1, member_to - member_from - 1);
      size_t separator_pos =
          member_formated.find_first_of(kInnerMemberSeparator);

      Code code = std::stoi(member_formated.substr(0, separator_pos));
      Name name = member_formated.substr(separator_pos + 1);
      Member member(code, name);
      result.push_back(member);

      member_from =
          group_formated.find_first_of(kOpenMemberBracket, member_to + 1);
      member_to =
          group_formated.find_first_of(kCloseMemberBracket, member_to + 1);
    }

    return result;
  }

  static Name GetByCode(const std::string& string, const Code& code) {
    Group data = DeFormat(string);
    for (const auto& pair : data) {
      if (pair.first == code) {
        return pair.second;
      }
    }
    return Name("");
  }

 private:
  static const char kOpenGroupBracket = 'A';
  static const char kCloseGroupBracket = 'B';
  static const char kOpenMemberBracket = 'C';
  static const char kCloseMemberBracket = 'D';
  static const char kInnerMemberSeparator = 'E';
};

bool IsCodePresent(const std::string& string, size_t code) {
  Formater formater;
  auto result = formater.GetByCode(string, code);
  return (result.size() > 0);
}

std::string GetNameByCode(const std::string& string, size_t code) {
  Formater formater;
  auto name_encoded = formater.GetByCode(string, code);

  LatinEncoder encoder;
  auto name = encoder.Decode(name_encoded);

  return name;
}

auto GetCode(const std::string& string) {
  Hasher hasher;
  auto hash = hasher.Hash(string);
  return hash;
}

int main() {
  std::string password;
  std::string group_code;
  std::cout << "Enter your password: ";
  std::cin >> password;
  std::cout << "\nEnter the group code: ";
  std::cin >> group_code;
  auto code = GetCode(password);
  if (IsCodePresent(group_code, code)) {
    std::cout << "\nYour gift reciever is " << GetNameByCode(group_code, code) << "\n\n";
  } else {
    std::cout << "\nSorry, it seems there's an error\n\n";
  }
}