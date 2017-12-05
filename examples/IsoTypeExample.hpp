#include <algorithm>

#include <IsoType.hpp>

namespace 
{
  std::string alignRight(const std::string& val, size_t maxLength, char padChar = ' ')
  {
     // This function assumes the length was checked by the caller
    std::string ret{val};
    for (size_t i = 0; i < maxLength - val.size(); i++)
      ret += padChar;
     
    return ret;
  }

  std::string alignLeft(const std::string& val, size_t maxLength_, char padChar = ' ')
  {
    std::string ret(maxLength_ - val.size(), padChar);
    return ret.append(val.begin(), val.end());
  }
}

namespace isolib
{
  namespace example
  {
    // Custom IsoTypes to be used in testing/examples
    struct Alpha final: public IsoType
    {
      public:
        explicit Alpha(size_t length, bool isVar = false) :
          IsoType(length, isVar)
        {};

        void validate(const std::string& value) const override
        {
          if (value.size() > maxLength_)
            throw std::invalid_argument("Value length is larger than allowed one");

          static const std::regex validator{"[a-zA-Z0-9 ]+"};
          if (!std::regex_match(value, validator))
            throw std::invalid_argument("Invalid alpha value");
        }

      private:
        std::string addPadding(const std::string& value) const override
        {
          return alignRight(value, maxLength_);
        }
    };

    struct Numeric final: public IsoType
    {
      public:
        explicit Numeric(size_t length, bool isVar = false) :
          IsoType(length, isVar)
        {};

        void validate(const std::string& value) const override
        {
          if (value.size() > maxLength_)
            throw std::invalid_argument("Value length is larger than allowed one");

          static const std::regex validator{"[0-9]+"};
          if (!std::regex_match(value, validator))
            throw std::invalid_argument("Invalid numeric value");
        }

      private:
        std::string addPadding(const std::string& value) const override
        {
          return alignLeft(value, maxLength_, '0');
        }
    };
  }
}
