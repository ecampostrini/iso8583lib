#include <functional>
#include <memory>
#include <string>

#include <DataElement.hpp>
#include <DataElementFactory.hpp>
#include <IsoType.hpp>

using namespace isolib;
using namespace isolib::example;

namespace
{
  using DebPtr = std::unique_ptr<DataElementBase>;

  std::unique_ptr<DataElement<Numeric>> makeDe2()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 16);
    ret->setName("PrimaryAccountNumber");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe3()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 6);
    ret->setName("ProcessingCode");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe4()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 10);
    ret->setName("AmountTransaction");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe6()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 10);
    ret->setName("AmountCardHolderBilling");
    return ret;
  }

  std::unique_ptr<DataElement<DateTime>> makeDe7()
  {
    auto ret = std::make_unique<DataElement<DateTime>>(LengthType::Fixed, 19);
    ret->setName("TransmissionDateTime");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe8()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 10);
    ret->setName("AmountCardHolderBillingFee");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe11()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 6);
    ret->setName("SystemTraceAuditNumber");
    return ret;
  }

  std::unique_ptr<DataElement<Time>> makeDe12()
  {
    auto ret = std::make_unique<DataElement<Time>>(LengthType::Fixed, 8);
    ret->setName("TimeLocalTransaction");
    return ret;
  }

  std::unique_ptr<DataElement<Date>> makeDe13()
  {
    auto ret = std::make_unique<DataElement<Date>>(LengthType::Fixed, 10);
    ret->setName("DateLocalTransaction");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe18()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 4);
    ret->setName("MerchantType");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe22()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 3);
    ret->setName("PosEntryMode");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe41()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 8);
    ret->setName("TerminalId");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe42()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 7);
    ret->setName("MerchantId");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe43()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 3);
    ret->setName("Currency");
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe88()
  {
    auto ret = std::make_unique<DataElement<Numeric>>(LengthType::Fixed, 10);
    ret->setName("DebitsAmount");
    return ret;
  }
}

const CreationMap ExampleFactory::creationMap_ = CreationMap{
  {"DE2", makeDe2},
  {"DE3", makeDe3},
  {"DE4", makeDe4},
  {"DE6", makeDe6},
  {"DE7", makeDe7},
  {"DE8", makeDe8},
  {"DE11", makeDe11},
  {"DE12", makeDe12},
  {"DE13", makeDe13},
  {"DE18", makeDe18},
  {"DE22", makeDe22},
  {"DE41", makeDe41},
  {"DE42", makeDe42},
  {"DE43", makeDe43},
  {"DE88", makeDe88},
};

std::unique_ptr<DataElementBase> ExampleFactory::create(const std::string& id)
{
  const auto& it = creationMap_.find(id);

  if (it == std::end(creationMap_))
    throw std::runtime_error("There is no data element with Id " + id);
  else
    return it->second();
}
