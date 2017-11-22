#include <DataElement.hpp>
#include <IsoType.hpp>

#include <catch.hpp>

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
      if (value.size() > length_)
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

      for (size_t i = 0; i  < length_ - value.size(); i++)
      {
        ret += ' ';
      }

      return ret;
    }
  };

  struct Numeric final: public IsoType
  {
    explicit Numeric(size_t length, bool isVar = false) :
      IsoType(length, isVar)
    {};

    void validate(const std::string& value) const override
    {
      if (value.size() > length_)
        throw std::invalid_argument("Value length is larger than allowed one");

      static const std::regex validator{"[0-9]+"};
      if (!std::regex_match(value, validator))
        throw std::invalid_argument("Invalid numeric value");
    }

    private:

    std::string addPadding(const std::string& value) const override
    {
      std::string ret(length_ - value.size(), '0');
      return ret.append(value.begin(), value.end());
    }
  };
}

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
