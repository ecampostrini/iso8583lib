#include <algorithm>
#include <map>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include <DataElement.hpp>
#include <IsoType.hpp>
#include <Utils.hpp>

/*
  IsoMessage<VisaFactory> txnRequest(0100);

  txnRequest["DE7"] = isolib::VisaFactory("SequenceNumber")("123321");

  DataElementCompositePtr customerInfo = isolib::VisaFactory("ConsumerInfo");
  customerInfo->add(isolib::VisaFactory("ConsumerName")->set("Esteban Campostrini));
  customerInfo->add(isolib::VisaFactory("ConsumerAddress")->set("Calle falsa 123"));
  customerInfo->add(isolib::VisaFactory("ConsumerCity")->set("Villaguay, Entre Rios"));
  customerInfo->add(isolib::VisaFactory("ConsumerPostalCode")->set("3240");
  txnRequest["DE10"] = std::move(customerInfo);

  txnRequest["DE15"] = isolib::VisaFactory("Amount")->set("1500");

  const std::string message = txnRequest.get();
  send(message, ...);
  IsoMessage<VisaFactory> txnResponse(0110);
  txnResponse.read(is);
  for (const auto& kv : txnResponse)
  {
    std::cout << kv.first << ": " << kv.second << std::endl;
  }

*/
namespace // isolib::detail
{
  void validateMessageType(const std::string& mt)
  {
    if (mt.size() != 4)
      throw std::runtime_error("Message type has wrong length, should be 4");
    auto allDigits = std::all_of(mt.begin(), mt.end(), [](const char& c) {return c >= '0' && c <= '9';});
    if (!allDigits)
      throw std::runtime_error("Message type can only contain digits");
  }
}

namespace isolib
{
  template <typename DataElementFactory>
  class IsoMessage
  {
    public:
    enum class BitmapType : char { Binary, Hex };

    public:
    IsoMessage(const std::string& messageType, BitmapType binBitmap = BitmapType::Hex) :
      _bitmapType(binBitmap)
    {
      validateMessageType(messageType);
      strncpy(_messageType, messageType.data(), 4);
    }

    void setField(size_t pos, std::unique_ptr<DataElementBase>&& de)
    {
      if (pos < 2 || pos > 128)
        throw std::invalid_argument("Field index must be between 2 and 128");

      _fields[pos] = std::move(de);
      set(pos % 64 + 1, _bitmaps[pos/64]);
      if (pos >= 64)
        set(1, _bitmaps[0]);
    }

    std::string getField(size_t pos) const
    {
      if (pos < 2 || pos > 128)
      {
        throw std::invalid_argument("Field index must be between 2 and 128");
      }

      const auto it = _fields.find(pos);
      if (it == std::end(_fields))
        throw std::invalid_argument("Field is not present " + std::to_string(pos));

      return it->second->toString();
    }

    std::string write() const
    {
      std::ostringstream oss;
      auto writeBitmap = [&](int pos) -> void {
        if (_bitmapType == BitmapType::Binary)
          oss << toBinary(_bitmaps[pos]);
        else
          oss << toHex(_bitmaps[pos]);
      };

      oss << _messageType;
      writeBitmap(0);
      if (_bitmaps[1])
        writeBitmap(1);
      for (const auto& kv : _fields)
      {
        oss << kv.second->toString();
      }

      return oss.str();
    }

    // TODO implement some validation for the message type
    void read(const std::string& in)
    {
      std::istringstream iss{in};
      auto readBitmap = [&]() -> int64_t {
        if (_bitmapType == BitmapType::Binary)
          return 0; //fromBinary(readFixedField(iss, 8));
        else
          return 0; //fromHex(readFixedField(iss, 16));
      };

      strncpy(_messageType, readFixedField(iss, 4).data(), 4);
      _bitmaps[0] = readBitmap();
      auto firstBit = get(1, _bitmaps[0]);
      if (firstBit)
      {
        _bitmaps[1] = readBitmap();
      }

      for (auto i = 2; i < 64 + firstBit*64; i++)
      {
        if (!get(i, _bitmaps[i / 64]))
          continue;

        // TODO introduce a data element name policy by means of an extra
        // template argument, e.g.: NamePolicy::getName(i)
        auto debPtr = DataElementFactory::create("DE" + std::to_string(i));
        debPtr->parse(iss);
        _fields[i] = std::move(debPtr);
      }
    }

    private:
    char  _messageType[4];
    BitmapType _bitmapType;
    std::array<uint64_t, 2> _bitmaps;
    std::map<int, std::unique_ptr<DataElementBase>> _fields;
  };
}
