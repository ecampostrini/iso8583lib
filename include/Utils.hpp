#pragma once

#include <string>
#include <sstream>
#include <cassert>

#include <iostream>

namespace isolib
{
  namespace detail
  {
    const char hexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    template <typename T, bool U>
    struct SetImpl {};

    template <typename T>
    struct SetImpl<T, true>
    {
      static T set(size_t i, T target)
      {
        // TODO make the proper type checkings
        constexpr auto sizeInBits = sizeof(uint64_t) * 8;
        uint32_t mask = 1 << ((sizeInBits - i) % 32);
        if (i > 32)
        {
          return (target | static_cast<uint64_t>(mask));
        }
        else
        {
          uint32_t upperHalf = (target >> 32) | mask;
          uint32_t lowerHalf = target & 0xFFFFFFFF;
          return (static_cast<uint64_t>(upperHalf) << 32) | static_cast<uint64_t>(lowerHalf);
        }
      }
    };

    template <typename T>
    struct SetImpl<T, false>
    {
      static T set(size_t i, T target)
      {
        // TODO make the proper type checkings
        constexpr auto sizeInBits = sizeof(T) * 8;
        return (target | (1ULL << (sizeInBits - i)));
      }
    };
  }

  template <typename T>
  T set(size_t i, T target)
  {
    using UIntType = typename std::make_unsigned<T>::type;
    constexpr bool is64 = std::is_same<UIntType, uint64_t>::value;
    return detail::SetImpl<T, is64>::set(i, target);
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

  template <typename T>
  std::string toHex(T target)
  {
    static_assert(std::is_integral<T>::value, "The target has to be integral");

    constexpr const size_t numChars = (sizeof(T) * 8) / 4;
    char result[numChars];
    size_t currentChar = 0;

    for (int i = numChars - 1; i >= 0; i--)
    {
      const uint8_t acc = (target & (static_cast<T>(0xF) << 4 * i)) >> 4 * i;
      result[currentChar++] = detail::hexChars[acc];
    }

    return std::string{static_cast<const char*>(result), numChars};
  }

  std::string toBinary(uint64_t bitmap);
  std::string readFixedField(std::istringstream& iss, size_t length);
  std::string readVarField(std::istringstream& iss, size_t headerLength);
  size_t getNumberOfDigits(size_t number);
}
