#include <catch.hpp>

#include <DataElement.hpp>
#include <IsoType.hpp>
#include <IsoTypeExample.hpp>
#include <Utils.hpp>

using namespace IsoLib::Example;

using DebPtr = std::unique_ptr<DataElementBase>;

TEST_CASE("Test equality" , "[equality_test]")
{
    SECTION("Equality on simple data elements")
    {
      DataElement<Numeric> de1("123456", Numeric{10});
      DataElement<Numeric> de2("123456", Numeric{12});
      DataElement<Alpha> de3("123456", Alpha{10});
      DataElement<Numeric> de4("123456", Numeric{10});

      REQUIRE(de1 != de2);
      REQUIRE(de1 != de3);
      REQUIRE(de1 == de4);
    }

    SECTION("Equality on composite data elements")
    {}

    SECTION("Equality on decorated data elements")
    {}
}

TEST_CASE("Test toString", "[to_string_test]")
{
  // An alpha data element fully occupied
  DebPtr de1 = makeDataElementUnique("hello world", Alpha{11});
  // A numeric data element with padding
  DebPtr de2 = makeDataElementUnique("12345", Numeric{10});
  // An alpha data element with padding
  DebPtr de3 = makeDataElementUnique("hello", Alpha{10});

  SECTION("toString on simple data elements")
  {
    REQUIRE(de1->toString() == "hello world");
    REQUIRE(de2->toString() == "0000012345");
    REQUIRE(de3->toString() == "hello     ");
  }

  SECTION("toString on composite data elements")
  {}

  SECTION("toString on decorated data elements")
  {}
}

