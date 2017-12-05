#pragma once

#include <map>
#include <memory>
#include <string>
#include <sstream>

namespace isolib
{
  class DataElementBase
  {
    public:
      virtual ~DataElementBase() {};

      virtual std::string toString() const = 0;
      virtual void set(const std::string& value) = 0;
      virtual void parse(std::istringstream& iss) = 0;

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
  };

  template <typename IsoT>
  class DataElement : public DataElementBase
  {
    private:
      template<class IsoT2>
      friend class DataElement;

      std::string value_;
      IsoT type_;

    public:
      DataElement(IsoT t): type_(t) {};

      DataElement(const std::string& value, IsoT t) :
        value_(value),
        type_(t)
      {};

      std::string toString() const override
      {
        return type_.format(value_);
      }

      void set(const std::string& value) override
      {
        type_.validate(value);
        value_ = value;
      }

      virtual void parse(std::istringstream& iss) override 
      {
        value_ = type_.read(iss);
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

      std::string toString() const override
      {
        std::ostringstream os;
        for(const auto& de : subElements_)
        {
          os << de.second->toString();
        }

        return os.str();
      }

      [[noreturn]]
      void set(const std::string& value [[gnu::unused]]) override
      {
        throw std::runtime_error("set cannot be called on a Composite Data Element");
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
  };

  class DataElementDecorator : public DataElementBase
  {
    protected:
      std::unique_ptr<DataElementBase> deb_;
    public:
      DataElementDecorator(std::unique_ptr<DataElementBase> deb) :
        deb_(std::move(deb))
      {};

    private:
      virtual bool compare(const DataElementBase& other) const = 0;
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
}
