#include <iostream>

#include <DataElementFactory.hpp>
#include <IsoMessage.hpp>

using namespace isolib;
using namespace isolib::example;

int main()
{
  {
    auto dep5 = ExampleFactory::create("DE5");
    auto dep50 = ExampleFactory::create("DE50");
    auto dep94 = ExampleFactory::create("DE94");
    auto dep64 = ExampleFactory::create("DE64");
    dep5->setValue("125");
    dep50->setValue("esteban");
    dep94->setValue("10");
    dep64->setValue("armando estebanquito");

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
    dep5->setValue("125");
    dep50->setValue("esteban");
    dep94->setValue("10");
    dep64->setValue("armando estebanquito");

    IsoMessage<ExampleFactory> msg1("0100", BitmapType::Hex);
    msg1.setField(5, std::move(dep5));
    msg1.setField(50, std::move(dep50));
    msg1.setField(94, std::move(dep94));
    msg1.setField(64, std::move(dep64));

    std::string rawMsg = msg1.write();
    IsoMessage<ExampleFactory> msg2("0100", BitmapType::Hex);
    msg2.read(rawMsg);
    std::cout << std::boolalpha << (msg1.write() == msg2.write()) << std::endl;
    auto msg3(std::move(msg2));
    std::cout << msg3.write() << std::endl;
    std::cout << (msg3.write() == msg1.write()) << std::endl;
    const auto msg4 = std::move(msg3);
    std::cout << msg4.write() << std::endl;
    std::cout << (msg4.write() == msg1.write()) << std::endl;
    //message2 = message; // Compiler error
  }

  return 0;
}
