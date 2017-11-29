#pragma once

#include <DataElement.hpp>
#include <memory>

struct DataElementFactory
{
  DataElementFactory();
  virtual ~DataElementFactory() {};
  
  virtual std::unique_ptr<DataElementBase> createDataElement(const std::string& id) const = 0;    
  virtual bool registerNew(const std::string& id) const = 0;
  virtual bool unregister(const std::string& id) const = 0;
};

/*
struct ExampleFactory final : public DataElementFactory
{
  ExampleFactory() = default;
  ~ExampleFactory() {};

  std::unique_ptr<dataElementBase>

  private:
    std::map<std::string, std::function<std::unique_ptr<DataElementBase>(void)>> typeMap_;
};
*/
