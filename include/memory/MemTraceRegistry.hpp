#ifndef MEM_TRACE_REGISTRY_INCLUDED
#define MEM_TRACE_REGISTRY_INCLUDED

#include <memory>
#include <memory/Registry.hpp>
#include <memory/MemManager.hpp>

namespace Memory {

class MemTraceRegistry : public Registry {
  std::shared_ptr<MemManager> _manager;
  inline void checkPtr() { if (!_manager) throw "Set MemManager reference before using MemTraceRegistry."; }

public:
  inline MemTraceRegistry(std::shared_ptr<MemManager>& m) : _manager(m) {}

  inline void reg(RegistryItem& item) override { checkPtr(); _manager->reg(item); }
  inline void dereg(RegistryItem& item) override { checkPtr(); _manager->dereg(item); }
};

} // namespace Memory
#endif

