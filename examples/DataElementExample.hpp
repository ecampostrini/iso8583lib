#include <DataElement.hpp>

namespace isolib
{
  namespace example
  {
    class AdditionalDataElement : public DataElementDecorator
    {
      private:
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

        [[noreturn]]
        void set(const std::string& value [[gnu::unused]]) override
        {
          throw std::runtime_error("set cannot be called on an Addition Data Element");
        }

        void parse(std::istringstream& iss) override {}

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
  }
}
