#include <iostream>

#include <DataElementFactory.hpp>

using namespace IsoLib::Example;

int main()
{
  auto dep1 = ExampleFactory::createDataElement("DE1");
  auto dep2 = ExampleFactory::createDataElement("DE2");
  auto dep3 = ExampleFactory::createDataElement("DE3");
  dep1->set("125");
  dep2->set("esteban");
  dep3->set("10");
  
  std::cout << dep1->toString() << std::endl;
  std::cout << dep2->toString() << std::endl;
  std::cout << dep3->toString() << std::endl;

  return 0;
}
