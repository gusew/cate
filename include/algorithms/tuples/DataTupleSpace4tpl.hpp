#ifndef DATA_TUPLESPACE_4tpl_INCLUDED
#define DATA_TUPLESPACE_4tpl_INCLUDED

#include <memory>
#include <algorithms/common/Data4tpl.hpp>
#include <algorithms/tuples/DataTupleSpace.hpp>
#include <memory/MemTrace.hpp>
#include <core/ChronoManager.hpp>

namespace DataTupleSpace4tpl {

/** Represents a tuple with number of relevant bits of prefixes. */
struct Tuple4 {
  /** for 4x32bit - tuple: 4 x 6 bit = total width 24 bit */
  Memory::MemTrace<uint32_t> bitwidthmap;
  
  /** Create a bitmap with bitwidths of each tuple element stored in a single value. */
  Tuple4(size_t w1, 
    size_t w2, 
    size_t w3, 
    size_t w4) : bitwidthmap(0) 
  {
    // cut values to expected bit widths
    w1 &= 0x3F;
    w2 &= 0x3F;
    w3 &= 0x3F;
    w4 &= 0x3F;

    // move relevant bits to each position in result
    bitwidthmap |= ((uint32_t)w1 << 18);
    bitwidthmap |= ((uint32_t)w2 << 12);
    bitwidthmap |= ((uint32_t)w3 << 6);
    bitwidthmap |= (uint32_t)w4;
  }
  
  Tuple4(uint32_t map) : bitwidthmap(map) {}

  inline bool operator==(const Tuple4& oth) const { return bitwidthmap == oth.bitwidthmap; }

  inline size_t getWidth1() const { return ((size_t)(bitwidthmap >> 18)) & 0x3F; }
  inline size_t getWidth2() const { return ((size_t)(bitwidthmap >> 12)) & 0x3F; }
  inline size_t getWidth3() const { return ((size_t)(bitwidthmap >> 6)) & 0x3F; }
  inline size_t getWidth4() const { return (size_t)bitwidthmap & 0x3F; }
};

/** Tuple structure for common five tuple. */
typedef std::unique_ptr<Tuple4> Tuple4ptr;

/** Represents a 4 tuple rule with an index and arbitrary types. */
template <typename S>
struct PrefixRule {
  Memory::MemTrace<size_t> index; // rule index
  DataTupleSpace::Prefix<S> check1;
  DataTupleSpace::Prefix<S> check2;
  DataTupleSpace::Prefix<S> check3;
  DataTupleSpace::Prefix<S> check4;

  Tuple4ptr tuplemap;
  PrefixRule<S>* next;
  Memory::MemTrace<int*> _memdummy_next; // dummy object for tracing memory of next-pointer

  PrefixRule(const size_t idx, 
    const S& val1, const size_t width1,
    const S& val2, const size_t width2,
    const S& val3, const size_t width3,
    const S& val4, const size_t width4) : index(idx),
    check1(val1, width1),
    check2(val2, width2),
    check3(val3, width3),
    check4(val4, width4),
    tuplemap(new Tuple4(width1, width2, 
      width3, width4)),
    next(nullptr)
  {}

  ~PrefixRule() { if (next != nullptr) delete next; }

  /** Calculate a hash value for the complete rule. */
  uint32_t getHash() const {
    return (uint32_t)check1.shiftval ^ (uint32_t)check2.shiftval;
  }
   
  /** Returns true, if the header values are matched by this rule. */
  inline bool match(const S& val1, 
    const S& val2, 
    const S& val3, 
    const S& val4) const {

    return check1.match(val1) && 
      check2.match(val2) && 
      check3.match(val3) && 
      check4.match(val4);
  }
};

/** Complete rule in prefix notation for the common five tuple structure. */
typedef PrefixRule<uint32_t> Rule4tpl;
typedef std::unique_ptr<Rule4tpl> Rule4ptr;

/** A HashMap for the Tuple Space Search algorithm. */
template <typename S>
class Map {
  /** Total amount of buckets. */
  const size_t _capacity;
  /** Tuple structure of this map. */
  Tuple4ptr _tuplemap;
  /** Array with all buckets. */
  PrefixRule<S>** _storage;
  Memory::MemTrace<int**> _memdummy_storage; // dummy object for mem tracing

  /** Stores amount of bits to shift each tuple element. */
  const size_t _shift1;
  const size_t _shift2;
  const size_t _shift3;
  const size_t _shift4;
  /** Is true, if amount of bits shifted is equal to size of type. */
  const bool _wildcard1;
  const bool _wildcard2;
  const bool _wildcard3;
  const bool _wildcard4;

  Memory::MemTrace<unsigned long int> _minIndex;

  /** Calculates the hash value for an incoming packet header. */
  uint32_t calcHash(const S& v1, const S& v2) const {
    // special case for full size, because of otw. undefined shifting operation
    return (_wildcard1 ? 0 : (uint32_t) (v1 >> _shift1)) ^
      (_wildcard2 ? 0 : (uint32_t) (v2 >> _shift2));
  }
   
public:
  Map(const size_t capacity, Tuple4ptr tuple, unsigned long int noIndex) : 
    _capacity(capacity),
    _tuplemap(std::move(tuple)),
    _storage(new PrefixRule<S>*[capacity]),
    _memdummy_storage(new int*[capacity]), // dummy for mem tracing
    _shift1(sizeof(S)*8 - _tuplemap->getWidth1()),
    _shift2(sizeof(S)*8 - _tuplemap->getWidth2()),
    _shift3(sizeof(S)*8 - _tuplemap->getWidth3()),
    _shift4(sizeof(S)*8 - _tuplemap->getWidth4()),
    _wildcard1(_shift1 == sizeof(S)*8),
    _wildcard2(_shift2 == sizeof(S)*8),
    _wildcard3(_shift3 == sizeof(S)*8),
    _wildcard4(_shift4 == sizeof(S)*8),
    _minIndex(noIndex)
    {
      for (size_t it=0; it < _capacity; ++it) 
        _storage[it] = nullptr;
    }

  ~Map() {
    for (size_t it=0; it < _capacity; ++it) 
      if (_storage[it] != nullptr) 
        delete _storage[it];

    delete[] _storage;

    delete[] _memdummy_storage; // dummy object for correct mem tracing
  }

  /** Place a new rule into the storage by using quadratic probing. */
  void insert(std::unique_ptr<PrefixRule<S>> ruleptr) {
    // update minimum index value
    if (ruleptr->index < _minIndex) _minIndex = ruleptr->index;

    uint32_t index = ruleptr->getHash() % _capacity;
    if (_storage[index] == nullptr) {
      _storage[index] = ruleptr.release();
    } 
    else if (_storage[index]->index > ruleptr->index) {
      // inserted rule comes before already existing one
      ruleptr->next = _storage[index]; 
      _storage[index] = ruleptr.release();
    }
    else { // insert into forward linked list after first rule
      PrefixRule<S>* prev = _storage[index];
      PrefixRule<S>* next = prev->next;

      while (next != nullptr) {
        if (ruleptr->index < next->index) { // insert before next rule
          ruleptr->next = next;
          break; // insert new rule after previous in list
        }

        prev = next;
        next = next->next;
      }
      
      // insert new rule at end of linked list
      prev->next = ruleptr.release();
    }
  }

  /** Check, if this map is responsible for the given rule by comparing the tuple structure. */
  inline bool responsible(const std::unique_ptr<PrefixRule<S>>& ruleptr) const {
    // consider only first two fields which results in less tuples
    const uint32_t mask = 0xFFFFF000;
    return (ruleptr->tuplemap->bitwidthmap & mask) == (_tuplemap->bitwidthmap & mask);
  }

  /** Search for matching rule and returning its index, if available. */
  unsigned long int lookup(const Data4tpl::HeaderTuple& header) {
    uint32_t index = calcHash(header.v1, header.v2) % _capacity;

    // iterate over list
    PrefixRule<S>* rule = _storage[index]; _memdummy_storage[index];
      
    while (rule != nullptr) {
      if (rule->match(header.v1, 
          header.v2, 
          header.v3, 
          header.v4)) {

          return rule->index;
        }
      rule = rule->next;
    }

    // nothing was found
    return Generic::noRuleIsMatching();
  }

  /**
   * Iterate over all prefix rules contained in map and increment some index values.
   * @param oldIndex all existing rules with index >= oldIndex will be incremented
   */
  void incrementIndex(size_t oldIndex) {
    for (size_t i=0; i < _capacity; ++i) {
      // iterate over list, if rule exists
      PrefixRule<S>* rule = _storage[i];
      
      while (rule != nullptr) {
        if (rule->index >= oldIndex) ++rule->index;
        rule = rule->next;
      }
    }
  }
  
  /**
   * Iterate over all prefix rules contained in map and decrement some index values.
   * @param oldIndex all existing rules with index > oldIndex will be decremented
   */
  void decrementIndex(size_t oldIndex) {
    for (size_t i=0; i < _capacity; ++i) {
      // iterate over list, if rule exists
      PrefixRule<S>* rule = _storage[i];
      
      while (rule != nullptr) {
        if (rule->index > oldIndex) --rule->index;
        rule = rule->next;
      }
    }
  }

  /** Remove all existing rules with the given index. */
  void remove(size_t index) {
    if (index == _minIndex) ++_minIndex;

    for (size_t i=0; i < _capacity; ++i) {
      if (_storage[i] != nullptr) {
        PrefixRule<S>* next = _storage[i]->next;

        if (_storage[i]->index == index) {
          // set next rule as current rule in list
          delete _storage[i]; // remove this
          _storage[i] = next;
          --i; continue; // repeat loop for same i
        }
        else { // iterate over list, if more rules exist
          PrefixRule<S>* prev = _storage[i];
          next = prev->next;
        
          while (next != nullptr) 
          {
            if (next->index == index) {
              prev->next = next->next;
              delete next;
              next = prev->next;
            } else {
              prev = next;
              next = prev->next;
            }
          }
        }
      } // end if
    }
  }
  
  /** Getter for minimum index contained in map. */
  inline unsigned long int getMinIndex() const { return _minIndex; }

  /** Compare different maps by their minimum index. */
  inline bool operator<(const Map<S>& oth) const { return _minIndex < oth.getMinIndex(); }
};

/** Map for the common five tuple. */
typedef Map<uint32_t> Map4tpl;
typedef std::unique_ptr<Map4tpl> Map4ptr;

} // namespace DataTupleSpace4tpl

#endif

