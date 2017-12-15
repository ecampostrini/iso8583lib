#include <catch.hpp>
#include <iostream>

#include <Utils.hpp>

using namespace isolib;

TEST_CASE("Test var-length field parser", "[varlength_field_parser_test]")
{
  std::istringstream iss;
  std::string lvar1{"33this is some text two digits long"};
  std::string lvar2{"10helloworld"};
  std::string lvar3(99, 'X');

  std::string llvar1(100, 'X');
  std::string llvar2(999, 'X');

  std::string lllvar1(1000, 'X');
  std::string lllvar2(9999, 'X');

  auto setStr = [&] (const std::string str) -> std::istringstream&
  {
    iss.str(str);
    return iss;
  };

  SECTION("Properly formed single var fields")
  {
    REQUIRE(isolib::readVarField(setStr(lvar1), 2) == "this is some text two digits long");
    REQUIRE(isolib::readVarField(setStr(lvar2), 2) == "helloworld");
    REQUIRE(isolib::readVarField(setStr("99" + lvar3), 2) == lvar3);

    REQUIRE(isolib::readVarField(setStr("100" + llvar1), 3) == llvar1);
    REQUIRE(isolib::readVarField(setStr("999" + llvar2), 3) == llvar2);

    REQUIRE(isolib::readVarField(setStr("1000" + lllvar1), 4) == lllvar1);
    REQUIRE(isolib::readVarField(setStr("9999" + lllvar2), 4) == lllvar2);
  }

  SECTION("Properly formed compound var fields")
  {
    {
      std::string compound{lvar2 + "99" + lvar3};
      REQUIRE(isolib::readVarField(setStr(compound), 2) == "helloworld");
      REQUIRE(isolib::readVarField(iss, 2) == lvar3);
    }

    {
      std::string compound{lvar2 + "100" + llvar1};
      REQUIRE(isolib::readVarField(setStr(compound), 2) == "helloworld");
      REQUIRE(isolib::readVarField(iss, 3) == llvar1);
    }
  }

  SECTION("Erroneous var fields")
  {

  }
}

TEST_CASE("Test var-field prefix generator", "[varlength_prefix_generator_test]")
{
  REQUIRE(isolib::getNumberOfDigits(0) == 1);
  REQUIRE(isolib::getNumberOfDigits(9) == 1);
  REQUIRE(isolib::getNumberOfDigits(99) == 2);
  REQUIRE(isolib::getNumberOfDigits(999) == 3);
  REQUIRE(isolib::getNumberOfDigits(9999) == 4);
}

TEST_CASE("Test bitmap utility functions", "[bitmap_utility_function_test]")
{
  SECTION("Test to{hex, binary}")
  {}

  SECTION("Test from{hex, binary}")
  {}

  SECTION("Test set/get}")
  {
    {
      uint8_t i8{0};
      i8 = set(1, i8);
      REQUIRE(toHex(i8) == std::string("80"));
      i8 = set(1, i8);
      REQUIRE(toHex(i8) == std::string("80"));
      i8 = set(2, i8);
      REQUIRE(toHex(i8) == std::string("C0"));
      i8 = set(8, i8);
      REQUIRE(toHex(i8) == std::string("C1"));
    }
    {
      uint32_t i32{0};
      i32 = set(1, i32);
      REQUIRE(toHex(i32) == std::string("80000000"));
      i32 = set(32, i32);
      REQUIRE(toHex(i32) == std::string("80000001"));
      i32 = set(17, i32);
      REQUIRE(toHex(i32) == std::string("80008001"));
    }
    {
      uint64_t i64{0};
      i64 = set(1, i64);
      REQUIRE(toHex(i64) == std::string("8000000000000000"));
      i64 = set(33, i64);
      REQUIRE(toHex(i64) == std::string("8000000080000000"));
      i64 = set(34, i64);
      REQUIRE(toHex(i64) == std::string("80000000C0000000"));
      i64 = set(64, i64);
      REQUIRE(toHex(i64) == std::string("80000000C0000001"));
    }
  }

}
