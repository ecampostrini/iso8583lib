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
    bool get(size_t idx) const
    {
      if (idx < 0 || idx >= 64 * N)
        throw std::invalid_argument("Trying to access an element out of the bounds of the bitmap");
      auto pos = idx % 64 == 0 ? 64 : idx % 64;
      return isolib::get(pos, _bitmap[idx / 64]);
    }

    void set(size_t idx)
    {
      if (idx <= 0 || idx > 64 * N)
        throw std::invalid_argument("Trying to set an element out of the bounds of the bitmap");
      auto pos = idx % 64 == 0 ? 64 : idx % 64;
      _bitmap[(idx + 1) / 64] = isolib::set(pos, _bitmap[(idx + 1) / 64]);
    }

    void clear(size_t idx)
    {
      if (idx <= 0 || idx > 64 * N)
        throw std::invalid_argument("Trying to clear an element out of the bounds of the bitmap");
      auto pos = idx % 64 == 0 ? 64 : idx % 64;
      // TODO implement clear
      //_bitmap[(idx + 1) / 64] = isolib::clear(pos, _bitmap[(idx + 1) / 64]);
    }

    // Does not validate the Type
    std::string getAs(BitmapType type) const
    {
      const auto& convFunc = type == BitmapType::Binary ? isolib::toBinary<uint64_t> : isolib::toHex<uint64_t>;
      auto operation = [&type, &convFunc](std::string& s, uint64_t val) -> std::string
      {
        if (!val) return s;
        return s.append(convFunc(val));
      };
      return std::accumulate(_bitmap.begin(), _bitmap.end(), std::string{}, operation);
    }

    // Does not validate the Type
    void readFrom(std::istringstream& iss, BitmapType type)
    {
      clear();

      const auto& convFunc = type == BitmapType::Binary ? isolib::fromBinary<uint64_t> : isolib::fromHex<uint64_t>;
      auto length = type == BitmapType::Binary ? 8 : 16;

      _bitmap[0] = convFunc(isolib::readFixedField(iss, length));
      size_t i = 1;
      while (this->get(i) && i < N)
      {
        _bitmap[i] = convFunc(isolib::readFixedField(iss, length));
        i++;
      }
    }

    void clear()
    {
      _bitmap.fill(0);
    }

    size_t size() const
    {
      return 64 * N;
    }

  private:
    std::array<uint64_t, N> _bitmap{{0}};
  };

  template <typename DataElementFactory, size_t BitmapLength = 2>
  class IsoMessage
  {
    public:
    IsoMessage(const std::string& messageType, BitmapType binBitmap = BitmapType::Hex) :
      _bitmapType(binBitmap)
    {
      static_assert(BitmapLength >= 1, "The bitmap length must be at least 1");
      validateMessageType(messageType);
      strncpy(_messageType, messageType.data(), 4);
    }

    IsoMessage(const IsoMessage& other) = default;
    IsoMessage(IsoMessage&& other) = default;
    IsoMessage& operator=(const IsoMessage& other) = default;
    IsoMessage& operator=(IsoMessage&& other) = default;

    std::shared_ptr<DataElementBase> getField(size_t id)
    {
      if (id < BitmapLength || id > _bitmap.size())
        throw std::invalid_argument("Field index must be between " + std::to_string(BitmapLength) +
              " and " + std::to_string(_bitmap.size()));

      const auto it = _fields.find(id);
      if (it != std::end(_fields))
        return _fields[id];

      _fields[id] = DataElementFactory::create("DE" + std::to_string(id));
      _bitmap.set(id);
      if (id > 64)
        // Indicate the nth bitmap as present
        _bitmap.set((id + 1) / 64);
      return _fields[id];
    }

    void setField(size_t pos, std::unique_ptr<DataElementBase>&& de)
    {
      if (pos < BitmapLength || pos > _bitmap.size())
      {
        throw std::invalid_argument("Field index must be between " +
            std::to_string(_bitmap.minPos()) + " and " + std::to_string(_bitmap.maxPos()));
      }

      _fields[pos] = std::move(de);
      _bitmap.set(pos);
      if (pos > 64)
        // Indicate the nth bitmap as present
        _bitmap.set((pos + 1) / 64);
    }

    std::string write() const
    {
      std::ostringstream oss;
      oss.write(_messageType, 4);
      oss << _bitmap.getAs(_bitmapType);
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
      strncpy(_messageType, readFixedField(iss, 4).data(), 4);
      _bitmap.readFrom(iss, _bitmapType);
      for (size_t i = BitmapLength; i < _bitmap.size(); i++)
      {
        if (!_bitmap.get(i))
          continue;
        auto debPtr = DataElementFactory::create("DE" + std::to_string(i));
        debPtr->parse(iss);
        _fields[i] = std::move(debPtr);
      }
    }

    void clear()
    {
      _bitmap.clear();
      _fields.clear();
    }

    private:
    char _messageType[4];
    BitmapType _bitmapType;
    Bitmap<BitmapLength> _bitmap;
    std::map<int, std::shared_ptr<DataElementBase>> _fields;
  };
}
