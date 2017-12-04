#include <IsoType.hpp>

namespace IsoLib
{
  namespace Example
  {
    // Custom IsoTypes to be used in testing/examples
    struct Alpha final: public IsoType
    {
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
        // This function assumes the length was checked by the caller
        std::string ret{value};

        for (size_t i = 0; i  < maxLength_ - value.size(); i++)
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
        if (value.size() > maxLength_)
          throw std::invalid_argument("Value length is larger than allowed one");

        static const std::regex validator{"[0-9]+"};
        if (!std::regex_match(value, validator))
          throw std::invalid_argument("Invalid numeric value");
      }

      private:

      std::string addPadding(const std::string& value) const override
      {
        std::string ret(maxLength_ - value.size(), '0');
        return ret.append(value.begin(), value.end());
      }
    };
  }
}
