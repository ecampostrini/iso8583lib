#pragma once 

#include <map>
#include <memory>
#include <string>
#include <sstream>

class DataElementBase
{
  public:
    virtual ~DataElementBase() {};
    virtual std::string toString() const = 0; 

    friend bool operator==(const DataElementBase& lhs, const DataElementBase& rhs)
    {
      return lhs.compare(rhs); 
    }


  private:
    virtual bool compare(const DataElementBase& other) const = 0;
};

bool operator!=(const DataElementBase& lhs, const DataElementBase& rhs)
{
  return !(lhs == rhs);
}

template <typename IsoT>
class DataElement : public DataElementBase
{
  private:
    template<class IsoT2>
    friend class DataElement;

    std::string value_;
    IsoT type_;

  public:
    DataElement(const std::string& value, IsoT t):
      value_(value),
      type_(t)
    {};

    std::string toString() const override
    {
      return type_.format(value_);
    }

    void set(const std::string& value)
    {
      if (value.length() > type_.length_)
        throw std::invalid_argument("Trying to set value larger than allowed");

      value_ = value;
    }
    
    template <typename IsoT2>
    bool operator==(const DataElement<IsoT2>& other) const
    {
      return std::is_same<IsoT, IsoT2>::value && 
             this->type_ == other.type_  && 
             this->value_ == other.value_;
    }

  private:
    virtual bool compare(const DataElementBase& other) const override
    {
      const auto rhs = dynamic_cast<const DataElement<IsoT>*>(&other);
      if (rhs)
      {
        return this->type_ == rhs->type_ && this->value_ == rhs->value_;
      }
      return false;
    }
};

class DataElementComposite final : public DataElementBase
{
  private:
    std::map<int, std::unique_ptr<DataElementBase>> subElements_;

  public:
    DataElementComposite() = default;
    
    ~DataElementComposite() {};

    DataElementComposite& add(int pos, std::unique_ptr<DataElementBase> de)
    {
      subElements_[pos] = std::move(de);
      return *this;
    }

    size_t remove(int pos)
    {
      return subElements_.erase(pos);
    }

    std::string toString() const override
    {
      std::ostringstream os;
      for(const auto& de : subElements_)
      {
        os << de.second->toString();
      }

      return os.str();
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
};

class DataElementDecorator : public DataElementBase
{
  protected:
    std::unique_ptr<DataElementBase> deb_;
  public:
    DataElementDecorator(std::unique_ptr<DataElementBase> deb) : deb_(std::move(deb))
    {};

    virtual std::string toString() const = 0;

  private: 
    virtual bool compare(const DataElementBase& other) const = 0;
};

class AdditionalDataElement : public DataElementDecorator
{
  const std::string prefix_;

  public:
    AdditionalDataElement(const std::string& prefix, std::unique_ptr<DataElementBase> deb) :
      DataElementDecorator(std::move(deb)),
      prefix_(prefix)
    {}

    std::string toString() const override
    {
      std::string ret{prefix_}; 
      ret += deb_->toString();
      ret += ";";

      return ret;
    }
  
  private:
    bool compare(const DataElementBase& other) const override
    {
      const auto rhs = dynamic_cast<const AdditionalDataElement*>(&other);
      if (rhs)
      {
        return this->prefix_ == rhs->prefix_ && *(this->deb_) == *(rhs->deb_);
      }

      return false;
    }
};

template <typename T>
DataElement<T> makeDataElement(const std::string& value, T isoType)
{
  return DataElement<T>(value, isoType);
}

template <typename T>
std::unique_ptr<DataElementBase> makeDataElementUnique(const std::string& value, T isoType)
{
  std::unique_ptr<DataElementBase> ret(new DataElement<T>(value, isoType));
  return ret;
}
