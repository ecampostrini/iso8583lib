#pragma once

#include <string>
#include <regex>

#include <Utils.hpp>

namespace isolib
{
  struct IsoType
  {
    protected:
      bool isVariable_;
      size_t maxLength_;

    public:
      IsoType(size_t length, bool isVariable) :
        isVariable_(isVariable),
        maxLength_(length)
      {
        if (!isVariable_ && length <= 0)
          throw std::logic_error("The lenght of an IsoType must be positive");
      }

      virtual ~IsoType() {};

      virtual std::string format(const std::string& value) const
      {
        validate(value);

        if (isVariable_)
        {
          const auto headerPrefixSize = getNumberOfDigits(maxLength_) - getNumberOfDigits(value.size());
          std::string ret(headerPrefixSize, '0');
          ret += std::to_string(value.size());
          ret += value; 

          return ret;
        }
        else
        {
          return addPadding(value);
        }
      }

      virtual std::string read(std::istringstream& iss) const
      {
        if (isVariable_)
        {
          return readVarField(iss, getNumberOfDigits(maxLength_)); 
        }
        else
        {
          return readFixedField(iss, maxLength_);
        }
      }

      virtual void validate(const std::string& value) const = 0;

      friend bool operator==(const IsoType& lhs, const IsoType& rhs)
      {
        return lhs.compare(rhs);
      }
      
      friend bool operator!=(const IsoType& lhs, const IsoType& rhs)
      {
        return !(lhs == rhs);
      }

    private:
      virtual std::string addPadding(const std::string& value) const
      {
        return value;
      }

      virtual bool compare(const IsoType& other) const
      {
        return this->isVariable_ == other.isVariable_ &&
               this->maxLength_ == other.maxLength_;
      }
  };
}


