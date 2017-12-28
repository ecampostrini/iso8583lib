#pragma once

#include <map>
#include <memory>
#include <string>
#include <sstream>

#include <Utils.hpp>

namespace isolib
{
  constexpr size_t MaxAllowedDataElementLength = 999;
  enum class LengthType : char { Fixed, Variable };


  class DataElementBase
  {
  public:
    DataElementBase(const std::string& name): _name(name) {};

    DataElementBase(): DataElementBase("_unnamed_") {};

    virtual ~DataElementBase() {};

    virtual std::string toString() const = 0;

    virtual void parse(std::istringstream& iss) = 0;

    virtual void setValue(const std::string& val) = 0;

    void setName(const std::string& newName)
    {
      _name = newName;
    }

    std::string getName() const
    {
      return _name;
    }

    friend bool operator==(const DataElementBase& lhs, const DataElementBase& rhs)
    {
      return lhs.compare(rhs);
    }

    friend bool operator!=(const DataElementBase& lhs, const DataElementBase& rhs)
    {
      return !(lhs == rhs);
    }
  private:
    virtual bool compare(const DataElementBase& other) const = 0;
  private:
    std::string _name;
  };


  template <typename Isotype>
  class DataElement: public DataElementBase
  {
  public:
    DataElement(const std::string& name = "_unnamed_",
      const std::string val = "",
      LengthType lType = LengthType::Variable,
      size_t maxLength = MaxAllowedDataElementLength):
        DataElementBase(name),
        _value(val),
        _lengthType(lType),
        _maxLength(maxLength)
    {
      if (!val.empty())
      {
        Isotype::validate(val);
        if (_value.size() > _maxLength)
          throw std::invalid_argument("Data element " + getName() + " has a maximum length of " + std::to_string(_maxLength));
      }
    }

    DataElement(const std::string& val, LengthType lType, size_t maxLength = MaxAllowedDataElementLength):
      DataElement("_unnamed_", val, lType, maxLength)
    {}

    DataElement(LengthType lType, size_t maxLength = MaxAllowedDataElementLength):
      DataElement("_unnamed_", "", lType, maxLength)
    {}

    void setValue(const std::string& newVal) override
    {
      Isotype::validate(newVal);
      if (newVal.size() > _maxLength)
        throw std::invalid_argument("Maximum length allowed for data element " + getName() + " is " + std::to_string(_maxLength));
      _value = newVal;
    }

    std::string getValue() const
    {
      return _value;
    }

    virtual std::string toString() const override
    {
      if (_lengthType == LengthType::Variable)
      {
        const auto headerPrefixSize = getNumberOfDigits(_maxLength) - getNumberOfDigits(_value.size());
        std::string ret(headerPrefixSize, '0');
        ret += std::to_string(_value.size());
        ret += _value;

        return ret;
      }
      else
      {
        std::string ret(_maxLength - _value.size(), Isotype::padChar);
        switch(Isotype::padType)
        {
          case PaddingType::Left:
            return ret + _value;
          case PaddingType::Right:
            return _value + ret;
          default:
            throw std::runtime_error("IsoType of data element " + getName() + " has invalid padding type");
        }
      }
    }

    virtual void parse(std::istringstream& iss) override
    {
      if (_lengthType == LengthType::Variable)
      {
        _value = readVarField(iss, getNumberOfDigits(_maxLength));
      }
      else
      {
        _value = readFixedField(iss, _maxLength);
      }

      try
      {
        Isotype::validate(_value);
      }
      catch(const std::invalid_argument& e)
      {
        throw std::runtime_error("While parsing input for data element " + getName() + ": " + e.what());
      }
    }

  private:
    virtual bool compare(const DataElementBase& other) const override
    {
      auto rhs = dynamic_cast<const DataElement*>(&other);
      if (rhs)
      {
        if (_lengthType != rhs->_lengthType)
          return false;
        if (_maxLength != rhs->_maxLength)
          return false;
        if (_value != rhs->_value)
          return false;
        // The name is not considered a fundamental attribute of a data element
        return true;
      }

      return false;
    }
  private:
    std::string _value;
    LengthType _lengthType;
    size_t _maxLength;
  };

  class DataElementComposite final : public DataElementBase
  {
  public:
    DataElementComposite(const std::string& name):
      DataElementBase(name)
    {};

    DataElementComposite() = default;

    ~DataElementComposite() {};

    std::string toString() const override
    {
      std::ostringstream os;
      for(const auto& de : subElements_)
      {
        os << de.second->toString();
      }

      return os.str();
    }

    void setValue(const std::string& newVal) override
    {
      std::istringstream iss{newVal};
      this->parse(iss);
    }

    void parse(std::istringstream& iss) override {}

    DataElementComposite& add(int pos, std::unique_ptr<DataElementBase> de)
    {
      subElements_[pos] = std::move(de);
      return *this;
    }

    size_t remove(int pos)
    {
      return subElements_.erase(pos);
    }
  private:
    virtual bool compare(const DataElementBase& other) const override
    {
      const auto rhs = dynamic_cast<const DataElementComposite*>(&other);
      if (rhs)
      {
        if (this->subElements_.size() != rhs->subElements_.size())
          return false;

        for (const auto& de : subElements_)
        {
          try
          {
            if (*de.second != *rhs->subElements_.at(de.first))
              return false;
          }
          catch(std::out_of_range& e)
          {
            return false;
          }
        }

        return true;
      }

      return false;
    }
  private:
    std::map<int, std::unique_ptr<DataElementBase>> subElements_;
  };

  class DataElementDecorator : public DataElementBase
  {
  public:
    DataElementDecorator(std::unique_ptr<DataElementBase> deb) :
      deb_(std::move(deb))
    {
      setName(deb->getName());
    };
  private:
    virtual bool compare(const DataElementBase& other) const = 0;
  protected:
    std::unique_ptr<DataElementBase> deb_;
  };
}
