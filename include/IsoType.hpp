#pragma once

#include <string>
#include <regex>
#include <stdexcept>

#include <Utils.hpp>
#include <Types.hpp>

// TODO sanitize str before including it in the exception message
#define MAKE_ISO_TYPE(tName, pType, pChar, pattern) \
struct  tName \
{ \
  static constexpr auto padType = (pType); \
  static constexpr auto padChar = (pChar); \
  static void validate(const std::string& str) \
  { \
    static const std::regex rgx{(pattern)}; \
    if (!std::regex_match(str, rgx)) \
      throw std::invalid_argument(str + " is not a valid " + #tName + " IsoType"); \
  } \
} \

namespace isolib
{
  MAKE_ISO_TYPE(Numeric, PaddingType::Left, '0', "[0-9]+");
  MAKE_ISO_TYPE(Alpha, PaddingType::Right, ' ', "[A-Za-z ]+");
  MAKE_ISO_TYPE(AlphaNumeric, PaddingType::Right, ' ', "[0-9A-Za-z ]+");
}
