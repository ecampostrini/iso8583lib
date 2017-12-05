#pragma once 

#include <string>
#include <sstream>
#include <cassert>

namespace isolib
{
  template <typename T>
  T set(size_t i, T target)
  {
    constexpr auto sizeInBits = sizeof(T) * 8;

    static_assert(std::is_integral<T>::value, "The target has to be integral and unsigned");
    assert(i <= sizeInBits && "The index cannot be bigger than the size in bits of the target");
    assert(i > 0 && "The minimum allowed value for the index is 1");

    return (target | (1UL << (sizeInBits - i)));
  }

  template <typename T>
  bool get(size_t i, T target)
  {
    constexpr auto sizeInBits = sizeof(T) * 8;

    static_assert(std::is_integral<T>::value, "The target has to be integral");
    assert(i <= sizeInBits && "The index cannot be bigger than the size in bits of the target");
    assert(i > 0 && "The minimum allowed value for the index is 1");

    return (target >> (sizeInBits - i)) & 1UL;
  }

  std::string toBinary(uint64_t bitmap);
  std::string toHex(uint64_t bitmap);
  std::string readFixedField(std::istringstream& iss, size_t length);
  std::string readVarField(std::istringstream& iss, size_t headerLength);
  size_t getNumberOfDigits(size_t number);
}
