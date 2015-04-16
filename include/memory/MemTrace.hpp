#ifndef MEM_TRACE_INCLUDED
#define MEM_TRACE_INCLUDED

#include <cstddef>
#include <memory>
#include <memory/Registry.hpp>
#include <memory/MemTraceRegistry.hpp>
#include <memory/MemTraceData.hpp>

namespace Memory {

/** Single point of truth (SPOT) for global variable */
extern std::shared_ptr<MemTraceRegistry> memRegistryPtr;

/** MemTrace-functionality can be disabled with g++-option "-D MEMTRACE_DISABLED" */
#ifndef MEMTRACE_DISABLED

/**
 * A wrapper for user-objects in order to measure allocated and accessed memory during
 * execution of filtering algorithms. The wrapper itself tries to be as transparent as
 * possible, so it inherits from the wrapped object.
 */
template <typename T>
class MemTrace : public T, public RegistryItem {
  MemTraceData _metadata;

public: 
  MemTrace() {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
  }
  
  /** copy c'tor */
  MemTrace(const T& other) : T(other) {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
    _metadata.write<T>();
  }

  MemTrace(T&& other) : T(other) {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
    _metadata.write<T>();
  }
  
  inline ~MemTrace() { memRegistryPtr->dereg(*this); } 

  const RegistryData& getData() const override { return _metadata; }
};

/** 
 * A wrapper for scalar values in order to measure allocated and accessed memory during
 * execution of filtering algorithms. The wrapper itself tries to be as transparent as
 * possible, so most available operators are overloaded and forwarded to operator-
 * behavior of the wrapped object.
 */
template <typename T> 
class MemTraceScalar : public RegistryItem {

protected:
  T _member; 
  MemTraceData _metadata;

public:
  MemTraceScalar() : _member() {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
  }

  /** copy c'tor */
  MemTraceScalar(const T& other) : _member(other) {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
    _metadata.write<T>();
  }

  MemTraceScalar(T&& other) : _member(other) {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
    _metadata.write<T>();
  }

  inline ~MemTraceScalar() { memRegistryPtr->dereg(*this); }

  const RegistryData& getData() const override { return _metadata; }

  // make pointer to wrapped object accessible from outside
  T* getPtr() { return &_member; } 

  // allow implicit casting operations (work around const-ness to log mem access)
  inline operator const T() const { const_cast<MemTraceData*>(&_metadata)->read<T>(); return _member; }

  // unary arithmetic ops
  inline MemTraceScalar<T>& operator++() { ++_member; _metadata.read<T>(); _metadata.write<T>(); return *this; } // prefix incr.
  inline MemTraceScalar<T> operator++(int) { _metadata.read<T>(); _metadata.write<T>(); return MemTraceScalar<T>(_member++); } // postfix incr.
  inline MemTraceScalar<T>& operator--() { --_member; _metadata.read<T>(); _metadata.write<T>(); return *this; } // prefix decr.
  inline MemTraceScalar<T> operator--(int) { _metadata.read<T>(); _metadata.write<T>(); return MemTraceScalar<T>(_member--); } // postfix decr.

  // binary arithmetic ops
  inline MemTraceScalar<T>& operator+=(const T& rhs) { _member += rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  inline MemTraceScalar<T>& operator-=(const T& rhs) { _member -= rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  inline MemTraceScalar<T>& operator*=(const T& rhs) { _member *= rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  inline MemTraceScalar<T>& operator/=(const T& rhs) { _member /= rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  inline MemTraceScalar<T>& operator%=(const T& rhs) { _member %= rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  inline MemTraceScalar<T>& operator&=(const T& rhs) { _member &= rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  inline MemTraceScalar<T>& operator^=(const T& rhs) { _member ^= rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  inline MemTraceScalar<T>& operator|=(const T& rhs) { _member |= rhs; _metadata.read<T>(); _metadata.write<T>(); return *this; }
  
  // unary operators
  inline MemTraceScalar<T> operator-() { _metadata.read<T>(); return MemTraceScalar<T>(-_member); } 
  inline MemTraceScalar<T> operator!() { _metadata.read<T>(); return MemTraceScalar<T>(!_member); } 
  inline MemTraceScalar<T> operator~() { _metadata.read<T>(); return MemTraceScalar<T>(~_member); } 
};

template <> class MemTrace<bool> : public MemTraceScalar<bool> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const bool& other) : MemTraceScalar(other) {}
  MemTrace(bool&& other) : MemTraceScalar(other) {}
  MemTrace<bool>& operator=(bool rhs) { 
    _member = rhs; _metadata.write<bool>();
    return *this;
  }
};
template <> class MemTrace<char> : public MemTraceScalar<char> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const char& other) : MemTraceScalar(other) {}
  MemTrace(char&& other) : MemTraceScalar(other) {}
  MemTrace<char>& operator=(char rhs) { 
    _member = rhs; _metadata.write<char>();
    return *this;
  }
};
template <> class MemTrace<char16_t> : public MemTraceScalar<char16_t> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const char16_t& other) : MemTraceScalar(other) {}
  MemTrace(char16_t&& other) : MemTraceScalar(other) {}
  MemTrace<char16_t>& operator=(char16_t rhs) { 
    _member = rhs; _metadata.write<char16_t>();
    return *this;
  }
};
template <> class MemTrace<char32_t> : public MemTraceScalar<char32_t> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const char32_t& other) : MemTraceScalar(other) {}
  MemTrace(char32_t&& other) : MemTraceScalar(other) {}
  MemTrace<char32_t>& operator=(char32_t rhs) { 
    _member = rhs; _metadata.write<char32_t>();
    return *this;
  }
};
template <> class MemTrace<wchar_t> : public MemTraceScalar<wchar_t> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const wchar_t& other) : MemTraceScalar(other) {}
  MemTrace(wchar_t&& other) : MemTraceScalar(other) {}
  MemTrace<wchar_t>& operator=(wchar_t rhs) { 
    _member = rhs; _metadata.write<wchar_t>();
    return *this;
  }
};
template <> class MemTrace<signed char> : public MemTraceScalar<signed char> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const signed char& other) : MemTraceScalar(other) {}
  MemTrace(signed char&& other) : MemTraceScalar(other) {}
  MemTrace<signed char>& operator=(signed char rhs) { 
    _member = rhs; _metadata.write<signed char>();
    return *this;
  }
};

template <> class MemTrace<short int> : public MemTraceScalar<short int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const short int& other) : MemTraceScalar(other) {}
  MemTrace(short int&& other) : MemTraceScalar(other) {}
  MemTrace<short int>& operator=(short int rhs) { 
    _member = rhs; _metadata.write<short int>();
    return *this;
  }
};
template <> class MemTrace<int> : public MemTraceScalar<int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const int& other) : MemTraceScalar(other) {}
  MemTrace(int&& other) : MemTraceScalar(other) {}
  MemTrace<int>& operator=(int rhs) { 
    _member = rhs; _metadata.write<int>();
    return *this;
  }
};
template <> class MemTrace<long int> : public MemTraceScalar<long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const long int& other) : MemTraceScalar(other) {}
  MemTrace(long int&& other) : MemTraceScalar(other) {}
  MemTrace<long int>& operator=(long int rhs) { 
    _member = rhs; _metadata.write<long int>();
    return *this;
  }
};
template <> class MemTrace<long long int> : public MemTraceScalar<long long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const long long int& other) : MemTraceScalar(other) {}
  MemTrace(long long int&& other) : MemTraceScalar(other) {}
  MemTrace<long long int>& operator=(long long int rhs) { 
    _member = rhs; _metadata.write<long long int>();
    return *this;
  }
};
template <> class MemTrace<unsigned char> : public MemTraceScalar<unsigned char> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned char& other) : MemTraceScalar(other) {}
  MemTrace(unsigned char&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned char>& operator=(unsigned char rhs) { 
    _member = rhs; _metadata.write<unsigned char>();
    return *this;
  }
};
template <> class MemTrace<unsigned short int> : public MemTraceScalar<unsigned short int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned short int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned short int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned short int>& operator=(unsigned short int rhs) { 
    _member = rhs; _metadata.write<unsigned short int>();
    return *this;
  }
};
template <> class MemTrace<unsigned int> : public MemTraceScalar<unsigned int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned int>& operator=(unsigned int rhs) { 
    _member = rhs; _metadata.write<unsigned int>();
    return *this;
  }
};
template <> class MemTrace<unsigned long int> : public MemTraceScalar<unsigned long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned long int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned long int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned long int>& operator=(unsigned long int rhs) { 
    _member = rhs; _metadata.write<unsigned long int>();
    return *this;
  }
};
template <> class MemTrace<unsigned long long int> : public MemTraceScalar<unsigned long long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned long long int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned long long int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned long long int>& operator=(unsigned long long int rhs) { 
    _member = rhs; _metadata.write<unsigned long long int>();
    return *this;
  }
};

template <> class MemTrace<float> : public MemTraceScalar<float> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const float& other) : MemTraceScalar(other) {}
  MemTrace(float&& other) : MemTraceScalar(other) {}
  MemTrace<float>& operator=(float rhs) { 
    _member = rhs; _metadata.write<float>();
    return *this;
  }
};
template <> class MemTrace<double> : public MemTraceScalar<double> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const double& other) : MemTraceScalar(other) {}
  MemTrace(double&& other) : MemTraceScalar(other) {}
  MemTrace<double>& operator=(double rhs) { 
    _member = rhs; _metadata.write<double>();
    return *this;
  }
};
template <> class MemTrace<long double> : public MemTraceScalar<long double> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const long double& other) : MemTraceScalar(other) {}
  MemTrace(long double&& other) : MemTraceScalar(other) {}
  MemTrace<long double>& operator=(long double rhs) { 
    _member = rhs; _metadata.write<long double>();
    return *this;
  }
};


/** 
 * A wrapper for pointer types in order to measure memory allocation and access. The wrapper
 * itself tries to be as transparent as possible, so pointer operators are overloaded
 * and passed to wrapped pointer.
 */
template <typename T>
class MemTracePtr : public RegistryItem{

protected:
  T _member;
  MemTraceData _metadata;

public:
  MemTracePtr() : _member() {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
  }

  MemTracePtr(T other) : _member(other) {
    _metadata.alloc<T>();
    memRegistryPtr->reg(*this);
    _metadata.write<T>();
  }

  inline ~MemTracePtr() { memRegistryPtr->dereg(*this); }

  const RegistryData& getData() const override { return _metadata; }

  // allow implicit casting operations (work around const-ness to log mem access)
  operator const T() const { const_cast<MemTraceData*>(&_metadata)->read<T>(); return _member; }

  inline decltype(*_member)& operator*() { _metadata.read<T>(); return *_member; }
  
  T operator->() { _metadata.read<T>(); return _member; }
  const T operator->() const { const_cast<MemTraceData*>(&_metadata)->read<T>(); return _member; }

  inline decltype(_member[0])& operator[](int idx) { _metadata.read<T>(); return _member[idx]; }
};

/** 
 * A specification for pointer types. Inherits from general pointer-template.
 */
template <typename T> 
class MemTrace<T*> : public MemTracePtr<T*> {
public:
  inline MemTrace() : MemTracePtr<T*>() {}
  inline MemTrace(T* other) : MemTracePtr<T*>(other) {}
};

/**
 * A wrapper for array types in order to measure memory allocation and access. The wrapper
 * itself tries to be as transparent as possible, so array operators are overloaded
 * and used for access of wrapped array elements.
 */
template <typename T, size_t N>
class MemTraceArr : public RegistryItem {

protected:
  T _member[N];
  MemTraceData _metadata;

public:
  MemTraceArr() : _member() { 
    _metadata.alloc<T>(N);
    memRegistryPtr->reg(*this); 
  }
  inline ~MemTraceArr() { memRegistryPtr->dereg(*this); }

  const RegistryData& getData() const override { return _metadata; }

  inline T& operator[](int idx) { _metadata.read<T>(); return _member[idx]; }
  inline const T& operator[](int idx) const { const_cast<MemTraceData*>(&_metadata)->read<T>(); return _member[idx]; }
};

/** 
 * A specification for array types. Inherits from general array-template.
 */
template <typename T, size_t N>
class MemTrace<T[N]> : public MemTraceArr<T, N> {
public:
  inline MemTrace() : MemTraceArr<T, N>() {}
};


#else // in case MemTrace-functionality is disabled

#ifndef MEM_TRACE_DATA_DUMMY 
#define MEM_TRACE_DATA_DUMMY
static MemTraceData _memTraceDataDummy;
#endif

template <typename T>
class MemTrace : public T, public RegistryItem {
public: 
  MemTrace() {}
  MemTrace(const T& other) : T(other) {}
  MemTrace(T&& other) : T(other) {}
  ~MemTrace() {}
  const RegistryData& getData() const override { return _memTraceDataDummy; } 
};

template <typename T> 
class MemTraceScalar : public RegistryItem {
protected:
  T _member; 

public:
  MemTraceScalar() : _member() {}
  MemTraceScalar(const T& other) : _member(other) {}
  MemTraceScalar(T&& other) : _member(other) {}
  ~MemTraceScalar() {}
  const RegistryData& getData() const override { return _memTraceDataDummy; }

  // make pointer to wrapped object accessible from outside
  T* getPtr() { return &_member; } 

  // allow implicit casting operations (work around const-ness to log mem access)
  inline operator const T() const { return _member; }

  // unary arithmetic ops
  inline MemTraceScalar<T>& operator++() { ++_member; return *this; } // prefix incr.
  inline MemTraceScalar<T> operator++(int) { return MemTraceScalar<T>(_member++); } // postfix incr.
  inline MemTraceScalar<T>& operator--() { --_member; return *this; } // prefix decr.
  inline MemTraceScalar<T> operator--(int) { return MemTraceScalar<T>(_member--); } // postfix decr.

  // binary arithmetic ops
  inline MemTraceScalar<T>& operator+=(const T& rhs) { _member += rhs; return *this; }
  inline MemTraceScalar<T>& operator-=(const T& rhs) { _member -= rhs; return *this; }
  inline MemTraceScalar<T>& operator*=(const T& rhs) { _member *= rhs; return *this; }
  inline MemTraceScalar<T>& operator/=(const T& rhs) { _member /= rhs; return *this; }
  inline MemTraceScalar<T>& operator%=(const T& rhs) { _member %= rhs; return *this; }
  inline MemTraceScalar<T>& operator&=(const T& rhs) { _member &= rhs; return *this; }
  inline MemTraceScalar<T>& operator^=(const T& rhs) { _member ^= rhs; return *this; }
  inline MemTraceScalar<T>& operator|=(const T& rhs) { _member |= rhs; return *this; }
  
  // unary operators
  inline MemTraceScalar<T> operator-() { return MemTraceScalar<T>(-_member); } 
  inline MemTraceScalar<T> operator!() { return MemTraceScalar<T>(!_member); } 
  inline MemTraceScalar<T> operator~() { return MemTraceScalar<T>(~_member); } 
};

template <> class MemTrace<bool> : public MemTraceScalar<bool> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const bool& other) : MemTraceScalar(other) {}
  MemTrace(bool&& other) : MemTraceScalar(other) {}
  MemTrace<bool>& operator=(bool rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<char> : public MemTraceScalar<char> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const char& other) : MemTraceScalar(other) {}
  MemTrace(char&& other) : MemTraceScalar(other) {}
  MemTrace<char>& operator=(char rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<char16_t> : public MemTraceScalar<char16_t> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const char16_t& other) : MemTraceScalar(other) {}
  MemTrace(char16_t&& other) : MemTraceScalar(other) {}
  MemTrace<char16_t>& operator=(char16_t rhs) { 
    _member = rhs; 
    return *this;
  }
};
template <> class MemTrace<char32_t> : public MemTraceScalar<char32_t> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const char32_t& other) : MemTraceScalar(other) {}
  MemTrace(char32_t&& other) : MemTraceScalar(other) {}
  MemTrace<char32_t>& operator=(char32_t rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<wchar_t> : public MemTraceScalar<wchar_t> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const wchar_t& other) : MemTraceScalar(other) {}
  MemTrace(wchar_t&& other) : MemTraceScalar(other) {}
  MemTrace<wchar_t>& operator=(wchar_t rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<signed char> : public MemTraceScalar<signed char> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const signed char& other) : MemTraceScalar(other) {}
  MemTrace(signed char&& other) : MemTraceScalar(other) {}
  MemTrace<signed char>& operator=(signed char rhs) { 
    _member = rhs; 
    return *this;
  }
};

template <> class MemTrace<short int> : public MemTraceScalar<short int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const short int& other) : MemTraceScalar(other) {}
  MemTrace(short int&& other) : MemTraceScalar(other) {}
  MemTrace<short int>& operator=(short int rhs) { 
    _member = rhs; 
    return *this;
  }
};
template <> class MemTrace<int> : public MemTraceScalar<int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const int& other) : MemTraceScalar(other) {}
  MemTrace(int&& other) : MemTraceScalar(other) {}
  MemTrace<int>& operator=(int rhs) { 
    _member = rhs; 
    return *this;
  }
};
template <> class MemTrace<long int> : public MemTraceScalar<long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const long int& other) : MemTraceScalar(other) {}
  MemTrace(long int&& other) : MemTraceScalar(other) {}
  MemTrace<long int>& operator=(long int rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<long long int> : public MemTraceScalar<long long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const long long int& other) : MemTraceScalar(other) {}
  MemTrace(long long int&& other) : MemTraceScalar(other) {}
  MemTrace<long long int>& operator=(long long int rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<unsigned char> : public MemTraceScalar<unsigned char> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned char& other) : MemTraceScalar(other) {}
  MemTrace(unsigned char&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned char>& operator=(unsigned char rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<unsigned short int> : public MemTraceScalar<unsigned short int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned short int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned short int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned short int>& operator=(unsigned short int rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<unsigned int> : public MemTraceScalar<unsigned int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned int>& operator=(unsigned int rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<unsigned long int> : public MemTraceScalar<unsigned long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned long int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned long int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned long int>& operator=(unsigned long int rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<unsigned long long int> : public MemTraceScalar<unsigned long long int> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const unsigned long long int& other) : MemTraceScalar(other) {}
  MemTrace(unsigned long long int&& other) : MemTraceScalar(other) {}
  MemTrace<unsigned long long int>& operator=(unsigned long long int rhs) { 
    _member = rhs;
    return *this;
  }
};

template <> class MemTrace<float> : public MemTraceScalar<float> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const float& other) : MemTraceScalar(other) {}
  MemTrace(float&& other) : MemTraceScalar(other) {}
  MemTrace<float>& operator=(float rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<double> : public MemTraceScalar<double> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const double& other) : MemTraceScalar(other) {}
  MemTrace(double&& other) : MemTraceScalar(other) {}
  MemTrace<double>& operator=(double rhs) { 
    _member = rhs;
    return *this;
  }
};
template <> class MemTrace<long double> : public MemTraceScalar<long double> { 
public:
  MemTrace() : MemTraceScalar() {}
  MemTrace(const long double& other) : MemTraceScalar(other) {}
  MemTrace(long double&& other) : MemTraceScalar(other) {}
  MemTrace<long double>& operator=(long double rhs) { 
    _member = rhs;
    return *this;
  }
};

template <typename T>
class MemTracePtr : public RegistryItem{
protected:
  T _member;

public:
  MemTracePtr() : _member() {}
  MemTracePtr(T other) : _member(other) {}
  ~MemTracePtr() {}
  const RegistryData& getData() const override { return _memTraceDataDummy; }

  operator const T() const { return _member; }
  inline decltype(*_member)& operator*() { return *_member; }
  T operator->() { return _member; }
  const T operator->() const { return _member; }
  inline decltype(_member[0])& operator[](int idx) { return _member[idx]; }
};

template <typename T> 
class MemTrace<T*> : public MemTracePtr<T*> {
public:
  MemTrace() : MemTracePtr<T*>() {}
  MemTrace(T* other) : MemTracePtr<T*>(other) {}
};

template <typename T, size_t N>
class MemTraceArr : public RegistryItem {

protected:
  T _member[N];

public:
  MemTraceArr() : _member() {}
  ~MemTraceArr() {} 
  const RegistryData& getData() const override { return _memTraceDataDummy; }

  inline T& operator[](int idx) { return _member[idx]; }
  inline const T& operator[](int idx) const { return _member[idx]; }
};

template <typename T, size_t N>
class MemTrace<T[N]> : public MemTraceArr<T, N> {
public:
  inline MemTrace() : MemTraceArr<T, N>() {}
};
#endif // MemTrace enabled/disabled

} // namespace Memory
#endif

