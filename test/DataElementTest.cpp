#include <catch.hpp>

#include <DataElement.hpp>
#include <IsoType.hpp>
#include <Utils.hpp>

using namespace isolib;

using DebPtr = std::unique_ptr<DataElementBase>;

TEST_CASE("New Test equality" , "[equality_test]")
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
    {}

    SECTION("Equality on decorated data elements")
    {}
}

TEST_CASE("NewTest toString", "[to_string_test]")
{
  // An alpha data element fully occupied
  auto de1 = DataElement<Alpha>("hello world", LengthType::Fixed, 11);
  // A numeric data element with padding
  auto de2 = DataElement<Numeric>("12345", LengthType::Fixed, 11);
  // An alpha data element with padding
  auto de3 = DataElement<Alpha>("hello", LengthType::Fixed, 10);

  SECTION("toString on simple data elements")
  {
    REQUIRE(de1.toString() == "hello world");
    REQUIRE(de2.toString() == "00000012345");
    REQUIRE(de3.toString() == "hello     ");
  }

  SECTION("toString on composite data elements")
  {}

  SECTION("toString on decorated data elements")
  {}
}


TEST_CASE("New data elements", "[new_data_elements]")
{
  DataElement<Numeric> de1(LengthType::Fixed, 10);
  de1.setValue("1231231");
}
