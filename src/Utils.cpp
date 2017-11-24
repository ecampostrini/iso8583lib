#include <iostream>

#include <cassert>
#include <string>
#include <sstream>

#include <Utils.hpp>

namespace
{

  static const char hexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

}

namespace Isolib
{
  std::string toBinary(uint64_t bits)
  {
    unsigned char bitmap[8] = {0};
    auto pos = 7;
    auto currentChar = 7;

    for (size_t i = 0; i < sizeof(uint64_t) * 8; i++)
    {
      if (bits & (1 << (currentChar * 8 + pos)))
        bitmap[currentChar] |= (1 << pos);

      pos--;
      if (pos < 0)
      {
        pos = 7;
        currentChar--;
      }
    }

    return std::string{reinterpret_cast<const char*>(bitmap), 8};
  }

  std::string toHex(uint64_t bits)
  {
    char bitmap[16] = {0};
    auto pos = 1;
    auto currentChar = 0;
    uint8_t acc = 0;

    for (size_t i = 1; i <= sizeof(uint64_t) * 8; i++)
    {
      if (get(i, bits))
      {
        acc = set(pos + 4, acc);
      }

      pos++;

      if (pos > 4)
      {
        bitmap[currentChar] = hexChars[acc];
        currentChar++;
        pos = 1;
        acc = 0;
      }
    }

    return std::string{static_cast<const char*>(bitmap), 16};
  }

  std::string readVarField(std::istringstream& iss, size_t l)
  {
    if (l > 4)
      throw std::invalid_argument("readVarField: length specifier is too long");

    char buff[4];
    iss.read(buff, l);
    if (!iss.good() || iss.gcount() != static_cast<std::streamsize>(l))
    {
      throw std::runtime_error("readVarField: error while reading length specifier");
    }

    size_t length = 0;
    for(size_t i = 0; i < l; i++)
    {
      const int digit = buff[i] - '0';
      if (digit < 0 || digit > 9)
        throw std::runtime_error("The length specifier contains invalid characters");
      length = (length * 10) + digit;
    }

    std::string result(length, ' ');
    iss.read(&result[0], length);
    if (!iss.good() || iss.gcount() != static_cast<std::streamsize>(length))
    {
      throw std::runtime_error("readVarField: error while reading the field");
    }

    return result;
  }
}
