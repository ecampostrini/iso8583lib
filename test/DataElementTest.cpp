#include <catch.hpp>
#include <memory>

#include <DataElement.hpp>
#include <IsoType.hpp>
#include <Utils.hpp>

using namespace isolib;

using DebPtr = std::unique_ptr<DataElementBase>;

namespace
{
  std::string name{"Armando Estebanquito"};
  std::string countryName{"Argentina"};
  std::string postCode{"3240"};
  std::string address{"Pedro Goyena"};
  std::string phoneNumber{"00543455421234"};
}

TEST_CASE("Test equality" , "[equality_test]")
{
  SECTION("On data elements")
  {
    DataElement<Alpha> de1(name, LengthType::Fixed, name.size());
    DataElement<Alpha> de2(name, LengthType::Fixed, name.size() + 1);
    DataElement<Alpha> de3(name, LengthType::Variable, name.size());
    DataElement<Alpha> de4(name, LengthType::Variable, name.size() + 1);
    DataElement<AlphaNumeric> de5(name, LengthType::Fixed, name.size());

    REQUIRE(de1 != de2);
    REQUIRE(de1 != de3);
    REQUIRE(de1 != de4);
    REQUIRE(de2 != de3);
    REQUIRE(de2 != de4);
    REQUIRE(de3 != de4);
    REQUIRE(de1 != de5);
    REQUIRE(de1 == DataElement<Alpha>("Name", name, LengthType::Fixed, name.size()));
  }

  SECTION("On composite data elements")
  {
    {
      DataElementComposite dec1("Contact info");
      dec1.add(1, std::make_unique<DataElement<Alpha>>(countryName, LengthType::Fixed, countryName.size()));
      dec1.add(2, std::make_unique<DataElement<Numeric>>(postCode, LengthType::Fixed, postCode.size() + 3));
      dec1.add(3, std::make_unique<DataElement<Numeric>>(phoneNumber, LengthType::Fixed, phoneNumber.size() + 5));

      DataElementComposite dec2("Contact info 2");
      dec2.add(1, std::make_unique<DataElement<Alpha>>(countryName, LengthType::Fixed, countryName.size()));
      dec2.add(2, std::make_unique<DataElement<Numeric>>(postCode, LengthType::Fixed, postCode.size() + 3));
      dec2.add(3, std::make_unique<DataElement<Numeric>>(phoneNumber, LengthType::Fixed, phoneNumber.size() + 5));

      REQUIRE(dec1 == dec2);
      dec1.erase(1);
      REQUIRE(dec1 != dec2);
      dec2.erase(1);
      REQUIRE(dec1 == dec2);
    }

    {
      auto dec1 = std::make_unique<DataElementComposite>("Contact info");
      dec1->add(1, std::make_unique<DataElement<Alpha>>(countryName, LengthType::Fixed, countryName.size()));
      dec1->add(2, std::make_unique<DataElement<Numeric>>(postCode, LengthType::Fixed, postCode.size() + 3));
      dec1->add(3, std::make_unique<DataElement<Numeric>>(phoneNumber, LengthType::Fixed, phoneNumber.size() + 5));
      
      DataElementComposite dec2("Nested composite");
      dec2.add(1, std::make_unique<DataElement<Alpha>>(name, LengthType::Fixed, name.size() + 5));
      dec2.add(2, std::move(dec1));

      auto dec3 = std::make_unique<DataElementComposite>("Contact info");
      dec3->add(1, std::make_unique<DataElement<Alpha>>(countryName, LengthType::Fixed, countryName.size()));
      dec3->add(2, std::make_unique<DataElement<Numeric>>(postCode, LengthType::Fixed, postCode.size() + 3));
      dec3->add(3, std::make_unique<DataElement<Numeric>>(phoneNumber, LengthType::Fixed, phoneNumber.size() + 5));

      DataElementComposite dec4("Nested composite 2");
      dec4.add(1, std::make_unique<DataElement<Alpha>>(name, LengthType::Fixed, name.size() + 5));
      dec4.add(2, std::move(dec3));

      REQUIRE(dec2 == dec4);
    }
  }
}

TEST_CASE("Test toString", "[to_string_test]")
{
  SECTION("On data elements")
  {
    REQUIRE(DataElement<Alpha>(name, LengthType::Fixed, name.size()).toString() == name);
    REQUIRE(DataElement<Alpha>(name, LengthType::Fixed, name.size() + 5).toString() == name + "     ");
    REQUIRE(DataElement<Alpha>(name, LengthType::Variable, name.size()).toString() == std::to_string(name.size()).append(name));
    REQUIRE(DataElement<Alpha>(name, LengthType::Variable, name.size() + 5).toString() == std::to_string(name.size()).append(name));

    REQUIRE(DataElement<Numeric>(postCode, LengthType::Fixed, postCode.size()).toString() == postCode);
    REQUIRE(DataElement<Numeric>(postCode, LengthType::Fixed, postCode.size() + 3).toString() == std::string{"000"}.append(postCode));
    REQUIRE(DataElement<Numeric>(postCode, LengthType::Variable, postCode.size()).toString() == std::to_string(postCode.size()).append(postCode));
    REQUIRE(DataElement<Numeric>(postCode, LengthType::Variable, postCode.size() + 5).toString() == std::to_string(postCode.size()).append(postCode));
  }

  SECTION("On composite data elements")
  {
    auto sellerContactInfo = std::make_unique<DataElementComposite>("Seller_contact_info");
    sellerContactInfo->add(1, std::make_unique<DataElement<Alpha>>(countryName, LengthType::Fixed, countryName.size()));
    sellerContactInfo->add(2, std::make_unique<DataElement<AlphaNumeric>>(address, LengthType::Fixed, address.size() + 5));
    sellerContactInfo->add(3, std::make_unique<DataElement<Numeric>>(postCode, LengthType::Fixed, postCode.size() + 3));
    sellerContactInfo->add(4, std::make_unique<DataElement<AlphaNumeric>>(phoneNumber, LengthType::Variable, phoneNumber.size() + 5));

    DataElementComposite sellerInfo;
    sellerInfo.add(1, std::make_unique<DataElement<Alpha>>(name, LengthType::Fixed, name.size() + 10));
    sellerInfo.add(2, std::move(sellerContactInfo));
  
    const std::string expected = name + std::string(10, ' ') +
                                 countryName +
                                 address + std::string(5, ' ') + 
                                 std::string(3, '0') + postCode +
                                 std::to_string(phoneNumber.size()) + phoneNumber;

    REQUIRE(sellerInfo.toString() == expected);
  }
}
