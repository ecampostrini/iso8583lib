#include <cassert>
#include <string>
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
}
