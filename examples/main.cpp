#include <iostream>

#include <DataElementFactory.hpp>
#include <IsoMessage.hpp>

using namespace isolib;
using namespace isolib::example;

int main()
{
  IsoMessage<ExampleFactory> msg1("1100"); //
  try
  {
    msg1.getField(2)->setValue("5511545649342263"); // PAN
    msg1.getField(3)->setValue("123456"); // Processing code
    msg1.getField(4)->setValue("1234"); // Amount
    msg1.getField(6)->setValue("1234"); // Amount cardholder billing
    msg1.getField(7)->setValue("2018-01-29T10:15:50"); // Transmission date time
    msg1.getField(8)->setValue("17"); // Amount cardholder billing fee
    msg1.getField(11)->setValue("341"); // System trace audit number
    msg1.getField(12)->setValue("10:10:23"); // Time local transaction
    msg1.getField(13)->setValue("2018-01-26"); // Date local transaction
    msg1.getField(18)->setValue("003"); // Merchant type
    msg1.getField(22)->setValue("001"); // POS entry mode
    msg1.getField(41)->setValue("7"); // Terminal ID
    msg1.getField(42)->setValue("1204"); // Merchant ID
    msg1.getField(43)->setValue("978"); // Currency
    msg1.getField(88)->setValue("12345"); // Debits amount

    std::cout << msg1.write() << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cerr << "Exception caught while assembling msg1: " << e.what() << std::endl;
  }

  // Both messages hold pointers to the same data elements...not sure if this
  // is desirable :S
  const auto msg2 = msg1;

  assert(msg2.write() == msg1.write());

  msg1.getField(8)->setValue("321");
  assert(msg2.write() == msg1.write());

  IsoMessage<ExampleFactory> msg3("1100");
  msg3.read(msg1.write());
  assert(msg3.write() == msg1.write());

  return 0;
}
