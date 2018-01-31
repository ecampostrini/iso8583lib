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
      if (idx < 0 || idx >= 64 * N)
        throw std::invalid_argument("Trying to set an element out of the bounds of the bitmap");
      auto pos = idx % 64 == 0 ? 64 : idx % 64;
      _bitmap[idx / 64] = isolib::set(pos, _bitmap[idx / 64]);
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

    auto clear()
    {
      for (auto& b : _bitmap)
        b = 0;
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
      //auto normalizedPos = id > 64 ? id - 64 : id;
      //_bitmaps[id > 64] = set(normalizedPos, _bitmaps[id > 64]);
      _bitmapss.set(id);
      return _fields[id];
    }

    void setField(size_t pos, std::unique_ptr<DataElementBase>&& de)
    {
      if (pos < 2 || pos > 128)
        throw std::invalid_argument("Field index must be between 2 and 128");

      //if (pos > 64 && !_bitmaps[1])
        //_bitmaps[0] = set(1, _bitmaps[0]);

      _fields[pos] = std::move(de);
      //auto normalizedPos = pos > 64 ? pos - 64 : pos;
      //_bitmaps[pos > 64] = set(normalizedPos, _bitmaps[pos > 64]);
      _bitmapss.set(pos);
    }

    std::string write() const
    {
      std::ostringstream oss;
      //auto writeBitmap = [&, this](int pos) -> void {
        //if (_bitmapType == BitmapType::Binary)
          //oss << toBinary(_bitmaps[pos]);
        //else
          //oss << toHex(_bitmaps[pos]);
      //};

      oss.write(_messageType, 4);
      //writeBitmap(0);
      //if (_bitmaps[1])
        //writeBitmap(1);
      oss << _bitmapss.getAs(_bitmapType);
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
      //auto readBitmap = [&, this](size_t bitmapNum) -> void {
        //if (_bitmapType == BitmapType::Binary)
          //_bitmaps[bitmapNum] = fromBinary<uint64_t>(readFixedField(iss, 8));
        //else
          //_bitmaps[bitmapNum] = fromHex<uint64_t>(readFixedField(iss, 16));
      //};

      //auto createFromBitmap = [&, this](size_t bitmapNum) -> void {
        //auto offset = bitmapNum ? 64 : 0;
        //size_t i = bitmapNum ? 1 : 2;

        //for (; i <= 64; i++)
        //{
          //if (!get(i, _bitmaps[bitmapNum]))
            //continue;
          //auto debPtr = DataElementFactory::create("DE" + std::to_string(i + offset));
          //debPtr->parse(iss);
          //_fields[i + offset] = std::move(debPtr);
        //}
      //};

      strncpy(_messageType, readFixedField(iss, 4).data(), 4);
      //readBitmap(0);
      //if (get(1, _bitmaps[0]))
        //readBitmap(1);
      //createFromBitmap(0);
      //if (get(1, _bitmaps[0]))
        //createFromBitmap(1);
      _bitmapss.readFrom(iss, _bitmapType);
      for (size_t i = 2; i < 128; i++)
      {
        if (!_bitmapss.get(i))
          continue;
        auto debPtr = DataElementFactory::create("DE" + std::to_string(i));
        debPtr->parse(iss);
        _fields[i] = std::move(debPtr);
      }
    }

    void clear()
    {
      //for (size_t i = 0; i < _bitmaps.size(); i++)
        //_bitmaps[i] = 0;
      _bitmapss.clear();
      _fields.clear();
    }

    private:
    char _messageType[4];
    BitmapType _bitmapType;
    Bitmap<2> _bitmapss;
    //std::array<uint64_t, 2> _bitmaps{{0, 0}};
    std::map<int, std::shared_ptr<DataElementBase>> _fields;
  };
}
