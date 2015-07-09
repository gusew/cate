#ifndef DATA_TUPLESPACE_10TPL_INCLUDED
#define DATA_TUPLESPACE_10TPL_INCLUDED

#include <memory>
#include <algorithms/common/Data10tpl.hpp>
#include <algorithms/tuples/DataTupleSpace.hpp>
#include <metering/memory/MemTrace.hpp>
#include <metering/time/ChronoManager.hpp>

namespace DataTupleSpace10tpl {

/** Represents a tuple with number of relevant bits of prefixes. */
struct Tuple10 {
  /** for 10x32bit - tuple: 10 x 6 bit = total width 60 bit */
  Memory::MemTrace<uint64_t> bitwidthmap;

  /** Create a bitmap with bitwidths of each tuple element stored in a single value. */
  Tuple10(size_t w1, 
    size_t w2, 
    size_t w3, 
    size_t w4,
    size_t w5,
    size_t w6,
    size_t w7,
    size_t w8,
    size_t w9,
    size_t w10) : bitwidthmap(0) 
  {
    // cut values to expected bit widths
    w1 &= 0x3F;
    w2 &= 0x3F;
    w3 &= 0x3F;
    w4 &= 0x3F;
    w5 &= 0x3F;
    w6 &= 0x3F;
    w7 &= 0x3F;
    w8 &= 0x3F;
    w9 &= 0x3F;
    w10 &= 0x3F;

    // move relevant bits to each position in result
    bitwidthmap |= ((uint64_t)w1 << 54);
    bitwidthmap |= ((uint64_t)w2 << 48);
    bitwidthmap |= ((uint64_t)w3 << 42);
    bitwidthmap |= ((uint64_t)w4 << 36);
    bitwidthmap |= ((uint64_t)w5 << 30);
    bitwidthmap |= ((uint64_t)w6 << 24);
    bitwidthmap |= ((uint64_t)w7 << 18);
    bitwidthmap |= ((uint64_t)w8 << 12);
    bitwidthmap |= ((uint64_t)w9 << 6);
    bitwidthmap |= (uint64_t)w10;
  }
  
  Tuple10(uint64_t map) : bitwidthmap(map) {}

  inline bool operator==(const Tuple10& oth) const { return bitwidthmap == oth.bitwidthmap; }

  inline size_t getWidth1() const { return ((size_t)(bitwidthmap >> 54)) & 0x3F; }
  inline size_t getWidth2() const { return ((size_t)(bitwidthmap >> 48)) & 0x3F; }
  inline size_t getWidth3() const { return ((size_t)(bitwidthmap >> 42)) & 0x3F; }
  inline size_t getWidth4() const { return ((size_t)(bitwidthmap >> 36)) & 0x3F; }
  inline size_t getWidth5() const { return ((size_t)(bitwidthmap >> 30)) & 0x3F; }
  inline size_t getWidth6() const { return ((size_t)(bitwidthmap >> 24)) & 0x3F; }
  inline size_t getWidth7() const { return ((size_t)(bitwidthmap >> 18)) & 0x3F; }
  inline size_t getWidth8() const { return ((size_t)(bitwidthmap >> 12)) & 0x3F; }
  inline size_t getWidth9() const { return ((size_t)(bitwidthmap >> 6)) & 0x3F; }
  inline size_t getWidth10() const { return ((size_t)bitwidthmap) & 0x3F; }
};

/** Tuple structure for common five tuple. */
typedef std::unique_ptr<Tuple10> Tuple10ptr;

/** Represents a 10 tuple rule with an index and arbitrary types. */
template <typename S>
struct PrefixRule {
  Memory::MemTrace<size_t> index; // rule index
  DataTupleSpace::Prefix<S> check1;
  DataTupleSpace::Prefix<S> check2;
  DataTupleSpace::Prefix<S> check3;
  DataTupleSpace::Prefix<S> check4;
  DataTupleSpace::Prefix<S> check5;
  DataTupleSpace::Prefix<S> check6;
  DataTupleSpace::Prefix<S> check7;
  DataTupleSpace::Prefix<S> check8;
  DataTupleSpace::Prefix<S> check9;
  DataTupleSpace::Prefix<S> check10;

  Tuple10ptr tuplemap;
  PrefixRule<S>* next;
  Memory::MemTrace<int*> _memdummy_next; // dummy object for tracing memory of next-pointer

  PrefixRule(const size_t idx, 
    const S& val1, const size_t width1,
    const S& val2, const size_t width2,
    const S& val3, const size_t width3,
    const S& val4, const size_t width4,
    const S& val5, const size_t width5,
    const S& val6, const size_t width6,
    const S& val7, const size_t width7,
    const S& val8, const size_t width8,
    const S& val9, const size_t width9,
    const S& val10, const size_t width10) : index(idx),
    check1(val1, width1),
    check2(val2, width2),
    check3(val3, width3),
    check4(val4, width4),
    check5(val5, width5),
    check6(val6, width6),
    check7(val7, width7),
    check8(val8, width8),
    check9(val9, width9),
    check10(val10, width10),
    tuplemap(new Tuple10(width1, width2, 
      width3, width4, 
      width5, width6,
      width7, width8, 
      width9, width10)),
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
    const S& val4, 
    const S& val5, 
    const S& val6, 
    const S& val7, 
    const S& val8, 
    const S& val9, 
    const S& val10) const {

    return check1.match(val1) && 
      check2.match(val2) && 
      check3.match(val3) && 
      check4.match(val4) && 
      check5.match(val5) && 
      check6.match(val6) && 
      check7.match(val7) && 
      check8.match(val8) && 
      check9.match(val9) && 
      check10.match(val10);
  }
};

/** Complete rule in prefix notation for the common five tuple structure. */
typedef PrefixRule<uint32_t> Rule10tpl;
typedef std::unique_ptr<Rule10tpl> Rule10ptr;

/** A HashMap for the Tuple Space Search algorithm. */
template <typename S>
class Map {
  /** Total amount of buckets. */
  const size_t _capacity;
  /** Tuple structure of this map. */
  Tuple10ptr _tuplemap;
  /** Array with all buckets. */
  PrefixRule<S>** _storage;
  Memory::MemTrace<int**> _memdummy_storage; // dummy object for mem tracing

  /** Stores amount of bits to shift each tuple element. */
  const size_t _shift1;
  const size_t _shift2;
  const size_t _shift3;
  const size_t _shift4;
  const size_t _shift5;
  const size_t _shift6;
  const size_t _shift7;
  const size_t _shift8;
  const size_t _shift9;
  const size_t _shift10;
  /** Is true, if amount of bits shifted is equal to size of type. */
  const bool _wildcard1;
  const bool _wildcard2;
  const bool _wildcard3;
  const bool _wildcard4;
  const bool _wildcard5;
  const bool _wildcard6;
  const bool _wildcard7;
  const bool _wildcard8;
  const bool _wildcard9;
  const bool _wildcard10;

  Memory::MemTrace<unsigned long int> _minIndex;

  /** Calculates the hash value for an incoming packet header. */
  uint32_t calcHash(const S& v1, const S& v2) const {
    // special case for full size, because of otw. undefined shifting operation
    return (_wildcard1 ? 0 : (uint32_t) (v1 >> _shift1)) ^ (_wildcard2 ? 0 : (uint32_t) (v2 >> _shift2));
  }
   
public:
  Map(const size_t capacity, Tuple10ptr tuple, unsigned long int noIndex) : 
    _capacity(capacity),
    _tuplemap(std::move(tuple)),
    _storage(new PrefixRule<S>*[capacity]),
    _memdummy_storage(new int*[capacity]), // dummy for mem tracing
    _shift1(sizeof(S)*8 - _tuplemap->getWidth1()),
    _shift2(sizeof(S)*8 - _tuplemap->getWidth2()),
    _shift3(sizeof(S)*8 - _tuplemap->getWidth3()),
    _shift4(sizeof(S)*8 - _tuplemap->getWidth4()),
    _shift5(sizeof(S)*8 - _tuplemap->getWidth5()),
    _shift6(sizeof(S)*8 - _tuplemap->getWidth6()),
    _shift7(sizeof(S)*8 - _tuplemap->getWidth7()),
    _shift8(sizeof(S)*8 - _tuplemap->getWidth8()),
    _shift9(sizeof(S)*8 - _tuplemap->getWidth9()),
    _shift10(sizeof(S)*8 - _tuplemap->getWidth10()),
    _wildcard1(_shift1 == sizeof(S)*8),
    _wildcard2(_shift2 == sizeof(S)*8),
    _wildcard3(_shift3 == sizeof(S)*8),
    _wildcard4(_shift4 == sizeof(S)*8),
    _wildcard5(_shift5 == sizeof(S)*8),
    _wildcard6(_shift6 == sizeof(S)*8),
    _wildcard7(_shift7 == sizeof(S)*8),
    _wildcard8(_shift8 == sizeof(S)*8),
    _wildcard9(_shift9 == sizeof(S)*8),
    _wildcard10(_shift10 == sizeof(S)*8),
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
    const uint64_t mask = 0xFFFF000000000000;
    // consider only first two fields which results in less tuples
    return (ruleptr->tuplemap->bitwidthmap & mask) == (_tuplemap->bitwidthmap & mask);
  }

  /** Search for matching rule and returning its index, if available. */
  unsigned long int lookup(const Data10tpl::HeaderTuple& header) {
    uint32_t index = calcHash(header.v1, header.v2) % _capacity;

    // iterate over list
    PrefixRule<S>* rule = _storage[index]; _memdummy_storage[index];
      
    while (rule != nullptr) {
      if (rule->match(header.v1, 
          header.v2, 
          header.v3, 
          header.v4, 
          header.v5, 
          header.v6, 
          header.v7, 
          header.v8, 
          header.v9, 
          header.v10)) {

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
typedef Map<uint32_t> Map10tpl;
typedef std::unique_ptr<Map10tpl> Map10ptr;

} // namespace DataTupleSpace10tpl

#endif

