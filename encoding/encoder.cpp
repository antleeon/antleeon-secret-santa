#pragma once
#include <algorithm>
#include <string>
#include <vector>

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