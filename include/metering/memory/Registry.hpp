#ifndef REGISTRY_INCLUDED
#define REGISTRY_INCLUDED

namespace Memory {

class RegistryData { 
public:
  virtual ~RegistryData() {}
};

class RegistryItem {
public:
  virtual ~RegistryItem() {}
  virtual const RegistryData& getData() const = 0;
};

class Registry {
public:
  virtual ~Registry() {}

  virtual void reg(RegistryItem&) = 0;
  virtual void dereg(RegistryItem&) = 0;
};

} // namespace Memory
#endif

