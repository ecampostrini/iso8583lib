#include <catch.hpp>
#include <memory>

#include <DataElement.hpp>
#include <IsoType.hpp>
#include <Utils.hpp>

using namespace isolib;

using DebPtr = std::unique_ptr<DataElementBase>;

TEST_CASE("Test equality" , "[equality_test]")
{
    SECTION("Equality on simple data elements")
    {
      DataElement<Numeric> de1("123456", LengthType::Fixed, 10);
      DataElement<Numeric> de2("123456", LengthType::Variable, 12);
      DataElement<AlphaNumeric> de3("123456", LengthType::Fixed, 10);
      DataElement<Numeric> de4("123456", LengthType::Fixed, 10);

      REQUIRE(de1 != de2);
      REQUIRE(de1 != de3);
      REQUIRE(de1 == de4);
    }

    SECTION("Equality on composite data elements")
    {
    }

    SECTION("Equality on decorated data elements")
    {}
}

TEST_CASE("Test toString", "[to_string_test]")
{
  std::string name{"Armando Estebanquito"};
  std::string countryName{"Argentina"};
  std::string postCode{"3240"};
  std::string address{"Pedro Goyena"};
  std::string phoneNumber{"00543455421234"};

  SECTION("Data Elements")
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

  SECTION("Composite data elements")
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
