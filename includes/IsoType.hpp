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

  protected:
  bool isVariable_;
  size_t length_;

  private:
  virtual std::string addPadding(const std::string& value) const
  {
    return value;
  }
};

struct Alpha final: public IsoType
{
  explicit Alpha(size_t length, bool isVar = false) :
    IsoType(length, isVar)
  {};
  
  void validate(const std::string& value) const override
  {
    if (value.size() > length_)
      throw std::invalid_argument("Value length is larger than allowed one");

    static const std::regex validator{"[a-zA-Z]+"};
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
