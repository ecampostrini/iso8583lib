#include <iostream>

#include <cassert>
#include <string>
#include <sstream>

#include <Utils.hpp>

namespace isolib
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

  std::string readFixedField(std::istringstream& iss, size_t length)
  {
    std::string result(length, ' ');
    iss.read(&result[0], length);
    if (!iss.good() || iss.gcount() != static_cast<std::streamsize>(length))
    {
      throw std::runtime_error("Error while reading field content");
    }
    return result;
  }

  std::string readVarField(std::istringstream& iss, size_t headerLength)
  {
    if (headerLength > 4)
      throw std::invalid_argument("readVarField: length specifier is too long");

    char buff[4];
    iss.read(buff, headerLength);
    if (!iss.good() || iss.gcount() != static_cast<std::streamsize>(headerLength))
    {
      throw std::runtime_error("readVarField: error while reading length specifier");
    }

    size_t length = 0;
    for(size_t i = 0; i < headerLength; i++)
    {
      const int digit = buff[i] - '0';
      if (digit < 0 || digit > 9)
        throw std::runtime_error("The length specifier contains invalid characters");
      length = (length * 10) + digit;
    }

    return readFixedField(iss, length);
  }

  size_t getNumberOfDigits(size_t number)
  {
    size_t digits = 1;

    while (number /= 10)
    {
      digits++;
    }

    return digits;
  }
}
