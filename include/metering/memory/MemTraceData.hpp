#ifndef MEM_TRACE_ACCESS_INCLUDED
#define MEM_TRACE_ACCESS_INCLUDED

#include <cstddef>
#include <metering/memory/Registry.hpp>

namespace Memory {

struct MemTraceData : public RegistryData {
  size_t allocBytes; // total allocation size
  size_t totalBytes; // total amount of bytes accessed
  size_t count; // amount of accesses, just increment on each
  size_t wBytes; // amount of bytes in write accesses
  size_t countWrite; // amount of write accesses, incr. on each write access

  MemTraceData() : allocBytes(0), totalBytes(0), count(0), wBytes(0), countWrite(0) {}
  
  template <typename T>
  void alloc() { allocBytes += sizeof(T); }
  template <typename T>
  void alloc(size_t amount) { allocBytes += amount*sizeof(T); }

  template <typename T>
  void read() { totalBytes += sizeof(T); ++count; }
  template <typename T>
  void write() { totalBytes += sizeof(T); ++count; wBytes += sizeof(T); ++countWrite; }

  template <typename T>
  void read(size_t amount) { totalBytes += amount*sizeof(T); count += amount; }
  template <typename T>
  void write(size_t amount) { totalBytes += amount*sizeof(T); count += amount; wBytes += amount*sizeof(T); countWrite += amount; }
};

} // namespace Memory
#endif

