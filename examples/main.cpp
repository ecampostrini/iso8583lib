#include <iostream>

#include <DataElementFactory.hpp>
#include <IsoMessage.hpp>

using namespace isolib;
using namespace isolib::example;

int main()
{
  auto dep5 = ExampleFactory::createDataElement("DE5");
  auto dep50 = ExampleFactory::createDataElement("DE50");
  auto dep94 = ExampleFactory::createDataElement("DE94");
  dep5->set("125");
  dep50->set("esteban");
  dep94->set("10");
  
  std::cout << dep5->toString() << std::endl;
  std::cout << dep50->toString() << std::endl;
  std::cout << dep94->toString() << std::endl;

  IsoMessage<ExampleFactory> message("0100");
  message.setField(5, std::move(dep5));
  message.setField(50, std::move(dep50));
  message.setField(94, std::move(dep94));

  std::cout << message.write() << std::endl;

  return 0;
}
