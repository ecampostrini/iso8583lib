#include <catch.hpp>
#include <iostream>

#include <IsoType.hpp>
#include <IsoTypeExample.hpp>

using Catch::Matchers::EndsWith;
using namespace isolib::example;

TEST_CASE("Test format", "[format_test]")
{
  {
    Alpha fixedAlpha{10};
    REQUIRE(fixedAlpha.format("hello") == "hello     ");
    REQUIRE_THROWS_WITH(fixedAlpha.format("overly long string"), EndsWith("Value length is larger than allowed one"));
  }
  {
    Alpha varAlpha{99, true};
    REQUIRE(varAlpha.format("abcd") == "04abcd");
    REQUIRE(varAlpha.format("this is some test string") == "24this is some test string");
  }
  {
    Alpha varAlpha{999, true}; 
    REQUIRE(varAlpha.format("abcd") == "004abcd");
    REQUIRE(varAlpha.format("this is some test string") == "024this is some test string");
    REQUIRE(varAlpha.format(std::string(457, 'X')) == "457" + std::string(457, 'X'));
  }
}

TEST_CASE("Test read", "[read_test]")
{
  std::string input;
  input += "hello     ";
  input += "04abcd";
  input += std::string(47, 'X');
  input += "87" + std::string(87, 'X');
  input += "578" + std::string(578, 'X');
  input += "001" + std::string(1, 'X');
  std::istringstream iss{input};
  Alpha fixed1(10);
  Alpha fixed2(47);
  Alpha var1(99, true);
  Alpha var2(999, true);

  REQUIRE(fixed1.read(iss) == "hello     ");
  REQUIRE(var1.read(iss) == "abcd");
  REQUIRE(fixed2.read(iss) == std::string(47, 'X'));
  REQUIRE(var1.read(iss) == std::string(87, 'X'));
  REQUIRE(var2.read(iss) == std::string(578, 'X'));
  REQUIRE(var2.read(iss) == "X");
}
