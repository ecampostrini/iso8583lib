#include <iostream>

#include <cassert>
#include <string>
#include <sstream>

#include <Utils.hpp>

namespace isolib
{
  int fromHex(char c)
  {
    switch(c)
    {
      case '0':
        return 0;
      case '1':
        return 1;
      case '2':
        return 2;
      case '3':
        return 3;
      case '4':
        return 4;
      case '5':
        return 5;
      case '6':
        return 6;
      case '7':
        return 7;
      case '8':
        return 8;
      case '9':
        return 9;
      case 'a':
      case 'A':
        return 10;
      case 'b':
      case 'B':
        return 11;
      case 'c':
      case 'C':
        return 12;
      case 'd':
      case 'D':
        return 13;
      case 'e':
      case 'E':
        return 14;
      case 'f':
      case 'F':
        return 15;
      default:
        throw std::invalid_argument("Not a hex char");
    }
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
