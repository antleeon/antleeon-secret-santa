#pragma once

#include "encoding/encoder.cpp"
#include "formater.cpp"
#include "generation/hasher.cpp"

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

auto GetDeFormatedGroup(const std::string& string) {
  Formater formater;
  auto deformated = formater.DeFormat(string);
  return deformated;
}

auto DecodeName(const std::string& string) {
  LatinEncoder encoder;
  auto decoded = encoder.Decode(string);
  return decoded;
}