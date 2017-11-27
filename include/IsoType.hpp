#pragma once
#include <string>
#include <regex>

#include <DataElement.hpp>

struct IsoType
{
  IsoType(size_t length, bool isVariable) :
    isVariable_(isVariable),
    maxLength_(length)
  {
    if (!isVariable_ && length <= 0)
      throw std::logic_error("The lenght of an IsoType must be positive");
  }

  virtual ~IsoType() {};

  std::string format(const std::string& value) const
  {
    validate(value);

    std::string ret;
    if (isVariable_)
    {
      ret = getVarFieldHeader(maxLength_, value.size()) + std::to_string(value.size());
      ret += value; 
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
  size_t maxLength_;

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

bool operator!=(const IsoType& lhs, const IsoType& rhs)
{
  return !(lhs == rhs);
}
