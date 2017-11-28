#include <IsoType.hpp>

#include <iostream>
#include <catch.hpp>

using Catch::Matchers::EndsWith;

namespace 
{
  // Custom IsoTypes to be used during the testing
  struct Alpha final: public IsoType
  {
    explicit Alpha(size_t length, bool isVar = false) :
      IsoType(length, isVar)
    {};

    void validate(const std::string& value) const override
    {
      if (value.size() > maxLength_)
        throw std::invalid_argument("Value length is larger than allowed one");

      static const std::regex validator{"[a-zA-Z ]+"};
      if (!std::regex_match(value, validator))
        throw std::invalid_argument("Invalid alpha value");
    }

    private:

    std::string addPadding(const std::string& value) const override
    {
      // This function assumes the length was checked by the caller
      std::string ret{value};

      for (size_t i = 0; i  < maxLength_ - value.size(); i++)
      {
        ret += ' ';
      }

      return ret;
    }
  };
}

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
