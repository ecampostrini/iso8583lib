#include <iostream>

#include <DataElementFactory.hpp>
#include <IsoMessage.hpp>
#include <Utils.hpp>

using namespace isolib;
using namespace isolib::example;

int main()
{
  {
    auto dep5 = ExampleFactory::create("DE5");
    auto dep50 = ExampleFactory::create("DE50");
    auto dep94 = ExampleFactory::create("DE94");
    auto dep64 = ExampleFactory::create("DE64");
    dep5->set("125");
    dep50->set("esteban");
    dep94->set("10");
    dep64->set("armando estebanquito");

    IsoMessage<ExampleFactory> message("0100");
    message.setField(5, std::move(dep5));
    message.setField(50, std::move(dep50));
    message.setField(94, std::move(dep94));
    message.setField(64, std::move(dep64));

    //std::cout << message.write() << std::endl;
  }

  {
    auto dep5 = ExampleFactory::create("DE5");
    auto dep50 = ExampleFactory::create("DE50");
    auto dep94 = ExampleFactory::create("DE94");
    auto dep64 = ExampleFactory::create("DE64");
    dep5->set("125");
    dep50->set("esteban");
    dep94->set("10");
    dep64->set("armando estebanquito");

    IsoMessage<ExampleFactory> message("0100");//, BitmapType::Binary);
    message.setField(5, std::move(dep5));
    message.setField(50, std::move(dep50));
    message.setField(94, std::move(dep94));
    message.setField(64, std::move(dep64));

    std::string rawMsg = message.write();
    std::cout << "Message 1:\n" << rawMsg << std::endl;
    IsoMessage<ExampleFactory> message2("0100");//, BitmapType::Binary);
    message2.read(rawMsg);
    std::cout << "Message2:\n" << message2.write() << std::endl;
    //message2 = message; // Compiler error

  }

  return 0;
}
