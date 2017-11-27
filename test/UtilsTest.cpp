#include <Utils.hpp>

#include <iostream>
#include <catch.hpp>

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
    REQUIRE(Isolib::readVarField(setStr(lvar1), 2) == "this is some text two digits long");
    REQUIRE(Isolib::readVarField(setStr(lvar2), 2) == "helloworld");
    REQUIRE(Isolib::readVarField(setStr("99" + lvar3), 2) == lvar3);

    REQUIRE(Isolib::readVarField(setStr("100" + llvar1), 3) == llvar1);
    REQUIRE(Isolib::readVarField(setStr("999" + llvar2), 3) == llvar2);

    REQUIRE(Isolib::readVarField(setStr("1000" + lllvar1), 4) == lllvar1);
    REQUIRE(Isolib::readVarField(setStr("9999" + lllvar2), 4) == lllvar2);
  }

  SECTION("Properly formed compound var fields")
  {
    {
      std::string compound{lvar2 + "99" + lvar3};
      REQUIRE(Isolib::readVarField(setStr(compound), 2) == "helloworld");
      REQUIRE(Isolib::readVarField(iss, 2) == lvar3);
    }

    {
      std::string compound{lvar2 + "100" + llvar1};
      REQUIRE(Isolib::readVarField(setStr(compound), 2) == "helloworld");
      REQUIRE(Isolib::readVarField(iss, 3) == llvar1);
    }
  }

  SECTION("Erroneous var fields")
  {

  }
}

TEST_CASE("Test var-field prefix generator", "[varlength_prefix_generator_test]")
{
  REQUIRE(Isolib::getVarFieldPrefix(0, 0) == "");
  REQUIRE(Isolib::getVarFieldPrefix(1, 1) == "");
  REQUIRE(Isolib::getVarFieldPrefix(5, 0) == "0");
  REQUIRE(Isolib::getVarFieldPrefix(50, 0) == "00");
  REQUIRE(Isolib::getVarFieldPrefix(50, 5) == "0");
  REQUIRE(Isolib::getVarFieldPrefix(50, 25) == "");
  REQUIRE(Isolib::getVarFieldPrefix(50, 50) == "");
  REQUIRE(Isolib::getVarFieldPrefix(100, 0) == "000");
  REQUIRE(Isolib::getVarFieldPrefix(100, 5) == "00");
  REQUIRE(Isolib::getVarFieldPrefix(100, 50) == "0");
  REQUIRE(Isolib::getVarFieldPrefix(100, 100) == "");
  REQUIRE(Isolib::getVarFieldPrefix(1000, 0) == "0000");
  REQUIRE(Isolib::getVarFieldPrefix(1000, 5) == "000");
  REQUIRE(Isolib::getVarFieldPrefix(1000, 50) == "00");
  REQUIRE(Isolib::getVarFieldPrefix(1000, 500) == "0");
  REQUIRE(Isolib::getVarFieldPrefix(1000, 1000) == "");

      
}
