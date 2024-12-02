#include <iostream>
#include <string>
#include <vector>

#include "encoding/encoder.cpp"
#include "formater.cpp"
#include "generation/hasher.cpp"
#include "generation/shuffle.cpp"
#include "getter.cpp"

using Name = std::string;
using EncodedName = std::string;
using Hash = size_t;

using MemberInputData = std::pair<Name, Hash>;
using GroupInputData = std::vector<MemberInputData>;

using MemberResultData = std::pair<Hash, EncodedName>;
using GroupResultData = std::vector<MemberResultData>;

GroupInputData ReadInput() {
  GroupInputData group_data;

  size_t group_size = 0;
  std::cout << "Enter the size of the group: ";
  std::cin >> group_size;
  std::cout << '\n';

  for (size_t i = 0; i < group_size; ++i) {
    Name name;
    std::cout << "Enter " << (i + 1) << " member name: ";
    std::cin >> name;

    Hash hash;
    std::cout << "Enter " << (i + 1) << " member code: ";
    std::cin >> hash;

    MemberInputData member_data(name, hash);
    group_data.push_back(member_data);
  }

  return group_data;
}

std::vector<Name> GetNames(const GroupInputData& input) {
  std::vector<Name> names;
  for (const auto& member : input) {
    names.push_back(member.first);
  }
  return names;
}

size_t GetSeed(const GroupInputData& input) {
  std::vector<Hash> hashes;
  for (const auto& member : input) {
    hashes.push_back(member.second);
  }

  Hasher hasher;
  auto hash = hasher.Hash(hashes, true);

  return hash;
}

std::vector<std::pair<Name, EncodedName>> GetPairs(
    const GroupInputData& input) {
  auto names = GetNames(input);
  auto seed = GetSeed(input);
  auto pairs = PairShuffled(names, seed);

  return pairs;
}

GroupResultData Generate(const GroupInputData& input) {
  GroupResultData result;
  auto pairs = GetPairs(input);

  std::vector<Name> pair_names;
  for (const auto& pair : pairs) {
    pair_names.push_back(pair.second);
  }
  LatinEncoder encoder;
  auto encoded_pair_names = encoder.Encode(pair_names);
  for (size_t i = 0; i < pairs.size(); ++i) {
    auto member_code = input[i].second;
    auto encoded_pair_name = encoded_pair_names[i];
    MemberResultData member_result(member_code, encoded_pair_name);
    result.push_back(member_result);
  }

  return result;
}

std::string ResultToString(const GroupResultData& result) {
  Formater formater;
  auto string = formater.Format(result);
  return string;
}

void PrintResultData(const GroupResultData& result) {
  std::cout << '\n';
  for (const auto& member : result) {
    std::cout << member.first << ": " << member.second << '\n';
  }
  std::cout << '\n';
}

bool CheckFormat(const GroupResultData& data, const std::string& string) {
  Formater formater;
  auto deformated = formater.DeFormat(string);
  return (data == deformated);
}

int main() {
  auto input_data = ReadInput();
  auto result_data = Generate(input_data);
  std::cout << "\nSuccessfully generated shuffle\n\n";
  PrintResultData(result_data);
  auto result_string = ResultToString(result_data);
  std::cout << ((CheckFormat(result_data, result_string))
                    ? "\nSuccessfully encoded data\n\n"
                    : "\nError while encoding data\n\n");
  std::cout << "\nEncoded string result:\n\n" << result_string << "\n\n";
}