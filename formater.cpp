#pragma once
#include <string>
#include <vector>

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