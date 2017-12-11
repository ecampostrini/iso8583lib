#include <map>
#include <iostream>
#include <memory>
#include <string>
#include <ostringstream>

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
namespace isolib
{
  template <typename DataElementFactory, size_t N = 2>
  class IsoMessage
  {
    //template <size_t N>
    //struct Bitmap
    //{
      //private:
      //std::array<int64_t, N> bitmaps;
    //}
    public:
    IsoMessage(int messageType, bool binBitmap = false) :
      _messageType(messageType),
      _binaryBitmap(binBitmap)
    {}

    void set(size_t pos, std::unique_ptr<DataElementBase>&& de)
    {
      if (pos < 2 || pos > 128)
        throw std::invalid_argument("Field index must be between 2 and 128");   
    }

    std::string get(size_t pos)
    {
      if (pos < 2 || pos > 128)
      {
        throw std::invalid_argument("Field index must be between 2 and 128");
      }

      const auto it = _fields.find(pos);
      if (pos == std::end(_fields))
        throw std::invalid_argument("Field is not present " + std::to_string(pos));

      return it->second->toString();
    }

    void write(std::ostringstream& oss) const
    {
    
    }

    void read(const std::string& in)
    {
       
    }

    std::string get() const
    {
    
    }

    private: 
    int _messageType;
    bool _binaryBitmap;
    std::array<int64_t, 2> _bitmaps;
    //Bitmap _bitmap;
    std::map<int, std::unique_ptr<DataElementBase>> _fields;
  };
}
