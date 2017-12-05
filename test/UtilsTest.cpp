#include <catch.hpp>
#include <iostream>

#include <Utils.hpp>

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
