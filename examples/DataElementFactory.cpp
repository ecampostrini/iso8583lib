#include <functional>
#include <memory>
#include <string>

#include <DataElement.hpp>
#include <DataElementFactory.hpp>
#include <IsoTypeExample.hpp>

using namespace isolib;
using namespace isolib::example;

namespace
{
  using DebPtr = std::unique_ptr<DataElementBase>;

  
  std::unique_ptr<DataElement<Alpha>> makeDe5()
  {
    std::unique_ptr<DataElement<Alpha>> ret;
    ret.reset(new DataElement<Alpha>{Alpha(20)});
    return ret;
  }

  std::unique_ptr<DataElement<Alpha>> makeDe50()
  {
    std::unique_ptr<DataElement<Alpha>> ret;
    ret.reset(new DataElement<Alpha>{Alpha(10)});
    return ret;
  }

  std::unique_ptr<DataElement<Numeric>> makeDe94()
  {
    std::unique_ptr<DataElement<Numeric>> ret;
    ret.reset(new DataElement<Numeric>{Numeric(10)});
    return ret;
  }

  std::unique_ptr<DataElement<Alpha>> makeDe64()
  {
    std::unique_ptr<DataElement<Alpha>> ret;
    ret.reset(new DataElement<Alpha>{Alpha(30)});
    return ret;
  }
}

const CreationMap ExampleFactory::creationMap_ = CreationMap{
  {"DE5", makeDe5},
  {"DE50", makeDe50},
  {"DE94", makeDe94},
  {"DE64", makeDe64}
};

std::unique_ptr<DataElementBase> ExampleFactory::createDataElement(const std::string& id)
{
  const auto& it = creationMap_.find(id);

  if (it == std::end(creationMap_))
    throw std::runtime_error("There is no data element with Id " + id);
  else
    return it->second();
}
