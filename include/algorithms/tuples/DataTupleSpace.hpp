#ifndef DATA_TUPLESPACE_INCLUDED
#define DATA_TUPLESPACE_INCLUDED

#include <metering/memory/MemTrace.hpp>
#include <iostream>

namespace DataTupleSpace {

/** Represents a rule atom in prefix notation. */
template <typename T>
struct Prefix {
  Memory::MemTrace<T> value;

  /** Stores amount of bits needed for rightshift. */
  //const size_t _shiftbits;
  Memory::MemTrace<size_t> _shiftbits;

  /** Is true, of width is equal to size of this type. */
  //const bool _wildcard;
  Memory::MemTrace<bool> _wildcard;

  /** The value of the prefix, already shifted by calculated amount of bits. */
  //const T shiftval;
  Memory::MemTrace<T> shiftval;

  Prefix(const T& val, const size_t w) : value(val), //width(w), 
    _shiftbits(sizeof(T)*8 - w), 
    _wildcard(_shiftbits == sizeof(T)*8),
    shiftval(_wildcard ? 0 : value >> _shiftbits) 
  {}

  inline bool match(const T& val) const {
    return _wildcard || ((val >> _shiftbits) == shiftval); 
  }
};

} // namespace DataTupleSpace

#endif

