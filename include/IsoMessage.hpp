#pragma once

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>

#include <DataElement.hpp>
#include <Utils.hpp>

namespace isolib
{
  enum class BitmapType : char { Binary, Hex };
  template <size_t N>
  class Bitmap
  {
  public:
    enum class Type : char {Binary, Hex};
  public:
    bool get(size_t idx) const
    {
      if (idx < 0 || idx >= 64 * N)
        throw std::invalid_argument("Trying to access an element out of the bounds of the bitmap"); 
      return get(idx % 64, _bitmap[idx / 64]);
    }

    void set(size_t idx) 
    {
      if (idx < 0 || idx >= 64 * N)
        throw std::invalid_argument("Trying to get an element out of the bounds of the bitmap");
      //_bitmap[idx / 64] = set(idx % 64, _bitmap[idx / 64]);
    }

    std::string getAs(Type type) const
    {
      if (type != Type::Binary && type != Type::Hex)
        throw std::invalid_argument("Invalid bitmap type: " + std::to_string(type));

      // Assumes type is valid
      auto operation = [&type](std::string& s, uint64_t val) -> std::string
      {
        if (type == Type::Binary)
          return s.append(toBinary(val));
        return s.append(toHex(val)); 
      };

      return std::accumulate(_bitmap.begin(), _bitmap.end(), std::string{}, operation);
    }

  private:
    std::array<uint64_t, N> _bitmap{{0}};
  };

  template <typename DataElementFactory>
  class IsoMessage
  {
    public:
    IsoMessage(const std::string& messageType, BitmapType binBitmap = BitmapType::Hex) :
      _bitmapType(binBitmap)
    {
      validateMessageType(messageType);
      strncpy(_messageType, messageType.data(), 4);
    }

    IsoMessage(const IsoMessage& other) = default;
    IsoMessage(IsoMessage&& other) = default;

    IsoMessage& operator=(const IsoMessage& other) = default;
    IsoMessage& operator=(IsoMessage&& other) = default;

    std::shared_ptr<DataElementBase> getField(size_t id)
    {
      if (id < 2 || id > 128)
        throw std::invalid_argument("Field index must be between 2 and 128");

      const auto it = _fields.find(id);
      if (it != std::end(_fields))
        return _fields[id];

      _fields[id] = DataElementFactory::create("DE" + std::to_string(id));
      auto normalizedPos = id > 64 ? id - 64 : id;
      _bitmaps[id > 64] = set(normalizedPos, _bitmaps[id > 64]);
      _bitmapss.set(id);
      return _fields[id];
    }

    void setField(size_t pos, std::unique_ptr<DataElementBase>&& de)
    {
      if (pos < 2 || pos > 128)
        throw std::invalid_argument("Field index must be between 2 and 128");

      if (pos > 64 && !_bitmaps[1])
        _bitmaps[0] = set(1, _bitmaps[0]);

      auto normalizedPos = pos > 64 ? pos - 64 : pos;
      _fields[pos] = std::move(de);
      _bitmaps[pos > 64] = set(normalizedPos, _bitmaps[pos > 64]);
    }

    std::string write() const
    {
      std::ostringstream oss;
      auto writeBitmap = [&, this](int pos) -> void {
        if (_bitmapType == BitmapType::Binary)
          oss << toBinary(_bitmaps[pos]);
        else
          oss << toHex(_bitmaps[pos]);
      };

      oss.write(_messageType, 4);
      writeBitmap(0);
      if (_bitmaps[1])
        writeBitmap(1);
      for (const auto& kv : _fields)
      {
        oss << kv.second->toString();
      }

      return oss.str();
    }

    // Right now it only works with at most 2 bitmaps
    void read(const std::string& in)
    {
      clear();

      std::istringstream iss{in};
      auto readBitmap = [&, this](size_t bitmapNum) -> void {
        if (_bitmapType == BitmapType::Binary)
          _bitmaps[bitmapNum] = fromBinary<uint64_t>(readFixedField(iss, 8));
        else
          _bitmaps[bitmapNum] = fromHex<uint64_t>(readFixedField(iss, 16));
      };

      auto createFromBitmap = [&, this](size_t bitmapNum) -> void {
        auto offset = bitmapNum ? 64 : 0;
        size_t i = bitmapNum ? 1 : 2;

        for (; i <= 64; i++)
        {
          if (!get(i, _bitmaps[bitmapNum]))
            continue;
          auto debPtr = DataElementFactory::create("DE" + std::to_string(i + offset));
          debPtr->parse(iss);
          _fields[i + offset] = std::move(debPtr);
        }
      };

      strncpy(_messageType, readFixedField(iss, 4).data(), 4);
      readBitmap(0);
      if (get(1, _bitmaps[0]))
        readBitmap(1);
      createFromBitmap(0);
      if (get(1, _bitmaps[0]))
        createFromBitmap(1);
    }

    void clear()
    {
      for (size_t i = 0; i < _bitmaps.size(); i++)
        _bitmaps[i] = 0;
      _fields.clear();
    }

    private:
    char _messageType[4];
    BitmapType _bitmapType;
    Bitmap<2> _bitmapss;
    std::array<uint64_t, 2> _bitmaps{{0, 0}};
    std::map<int, std::shared_ptr<DataElementBase>> _fields;
  };
}
