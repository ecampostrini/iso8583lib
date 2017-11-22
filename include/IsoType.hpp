#pragma once
#include <string>
#include <regex>

#include <DataElement.hpp>

struct IsoType
{
  IsoType(size_t length, bool isVariable) :
    isVariable_(isVariable),
    length_(length)
  {
    if (!isVariable_ && length <= 0)
      throw std::logic_error("The lenght of an IsoType must be positive");
  }

  virtual ~IsoType() {};
  
  std::string format(const std::string value) const
  {
    validate(value);

    std::string ret(length_, '0');
    if (isVariable_)
    {
      // compute the length 
    }
    else
    {
      ret = addPadding(value); 
    }

    return ret;
  }

  virtual void validate(const std::string& value) const = 0;

  friend bool operator==(const IsoType& lhs, const IsoType& rhs)
  {
    return lhs.compare(rhs);
  }

  protected:
  bool isVariable_;
  size_t length_;

  private:
  virtual std::string addPadding(const std::string& value) const
  {
    return value;
  }

  virtual bool compare(const IsoType& other) const
  {
    return this->isVariable_ == other.isVariable_ &&
           this->length_ == other.length_;
  }
};

bool operator!=(const IsoType& lhs, const IsoType& rhs)
{
  return !(lhs == rhs);
}
