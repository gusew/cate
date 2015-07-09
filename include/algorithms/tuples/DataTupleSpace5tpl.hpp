#ifndef DATA_TUPLESPACE_5TPL_INCLUDED
#define DATA_TUPLESPACE_5TPL_INCLUDED

#include <memory>
#include <algorithms/common/Data5tpl.hpp>
#include <algorithms/tuples/DataTupleSpace.hpp>
#include <memory/MemTrace.hpp>
#include <core/ChronoManager.hpp>

namespace DataTupleSpace5tpl {

/** Represents a tuple with number of relevant bits of prefixes. */
struct Tuple5 {
  /** for common 5-tuple: 6 bit, 6 bit, 5 bit, 5 bit, 4 bit = total width 26 bit */
  Memory::MemTrace<uint32_t> bitwidthmap;

  /** Create a bitmap with bitwidths of each tuple element stored in a single value. */
  Tuple5(size_t w1, 
    size_t w2, 
    size_t w3, 
    size_t w4,
    size_t w5) : bitwidthmap(0) 
  {
    // cut values to expected bit widths
    w1 &= 0x3F;
    w2 &= 0x3F;
    w3 &= 0x1F;
    w4 &= 0x1F;
    w5 &= 0x0F;
    // move relevant bits to each position in result
    bitwidthmap |= ((uint32_t)w1 << 20);
    bitwidthmap |= ((uint32_t)w2 << 14);
    bitwidthmap |= ((uint32_t)w3 << 9);
    bitwidthmap |= ((uint32_t)w4 << 4);
    bitwidthmap |= (uint32_t)w5;
  }
  
  Tuple5(uint32_t map) : bitwidthmap(map) {}

  inline bool operator==(const Tuple5& oth) const { return bitwidthmap == oth.bitwidthmap; }

  inline size_t getWidth1() const { return (size_t)bitwidthmap >> 20; }
  inline size_t getWidth2() const { return ((size_t)bitwidthmap >> 14) & 0x3F; }
  inline size_t getWidth3() const { return ((size_t)bitwidthmap >> 9) & 0x1F; }
  inline size_t getWidth4() const { return ((size_t)bitwidthmap >> 4) & 0x1F; }
  inline size_t getWidth5() const { return (size_t)bitwidthmap & 0x0F; }
};

/** Tuple structure for common five tuple. */
typedef std::unique_ptr<Tuple5> Tuple5ptr;

/** Represents a five tuple rule with an index and arbitrary types. */
template <typename S, typename T, typename U, typename V, typename W>
struct PrefixRule {
  Memory::MemTrace<size_t> index; // rule index
  DataTupleSpace::Prefix<S> check1;
  DataTupleSpace::Prefix<T> check2;
  DataTupleSpace::Prefix<U> check3;
  DataTupleSpace::Prefix<V> check4;
  DataTupleSpace::Prefix<W> check5;
  Tuple5ptr tuplemap;
  PrefixRule<S, T, U, V, W>* next;
  Memory::MemTrace<int*> _memdummy_next; // dummy object for tracing memory of next-pointer

  PrefixRule(const size_t idx, 
    const S& val1, const size_t width1,
    const T& val2, const size_t width2,
    const U& val3, const size_t width3,
    const V& val4, const size_t width4,
    const W& val5, const size_t width5) : index(idx),
    check1(val1, width1),
    check2(val2, width2),
    check3(val3, width3),
    check4(val4, width4),
    check5(val5, width5),
    tuplemap(new Tuple5(width1, width2, width3, width4, width5)),
    next(nullptr)
  {}

  ~PrefixRule() { if (next != nullptr) delete next; }

  /** Calculate a hash value for the complete rule. */
  uint32_t getHash() const {
    return (uint32_t)check1.shiftval ^ (uint32_t)check2.shiftval;
  }
   
  /** Returns true, if the header values are matched by this rule. */
  inline bool match(const S& val1, 
    const T& val2, 
    const U& val3, 
    const V& val4, 
    const W& val5) const {

    return check1.match(val1) && 
      check2.match(val2) && 
      check3.match(val3) && 
      check4.match(val4) && 
      check5.match(val5);
  }
};

/** Complete rule in prefix notation for the common five tuple structure. */
typedef PrefixRule<uint32_t, uint32_t, uint16_t, uint16_t, uint8_t> Rule5tpl;
typedef std::unique_ptr<Rule5tpl> Rule5ptr;

/** A HashMap for the Tuple Space Search algroithm. */
template <typename S, typename T, typename U, typename V, typename W>
class Map {
  /** Total amount of buckets. */
  const size_t _capacity;
  /** Tuple structure of this map. */
  Tuple5ptr _tuplemap;
  /** Array with all buckets. */
  PrefixRule<S, T, U, V, W>** _storage;
  Memory::MemTrace<int**> _memdummy_storage; // dummy object for mem tracing

  /** Stores amount of bits to shift each tuple element. */
  const size_t _shiftS;
  const size_t _shiftT;
  const size_t _shiftU;
  const size_t _shiftV;
  const size_t _shiftW;
  /** Is true, if amount of bits shifted is equal to size of type. */
  const bool _wildcardS;
  const bool _wildcardT;
  const bool _wildcardU;
  const bool _wildcardV;
  const bool _wildcardW;

  Memory::MemTrace<unsigned long int> _minIndex;

  /** Calculates the hash value for an incoming packet header. */
  uint32_t calcHash(const S& v1, const T& v2) const {
    // special case for full size, because of otw. undefined shifting operation
    return (_wildcardS ? 0 : (uint32_t) (v1 >> _shiftS)) ^ (_wildcardT ? 0 : (uint32_t) (v2 >> _shiftT));
  }
   
public:
  Map(const size_t capacity, Tuple5ptr tuple, unsigned long int noIndex) : 
    _capacity(capacity),
    _tuplemap(std::move(tuple)),
    _storage(new PrefixRule<S, T, U, V, W>*[capacity]),
    _memdummy_storage(new int*[capacity]), // dummy for mem tracing
    _shiftS(sizeof(S)*8 - _tuplemap->getWidth1()),
    _shiftT(sizeof(T)*8 - _tuplemap->getWidth2()),
    _shiftU(sizeof(U)*8 - _tuplemap->getWidth3()),
    _shiftV(sizeof(V)*8 - _tuplemap->getWidth4()),
    _shiftW(sizeof(W)*8 - _tuplemap->getWidth5()),
    _wildcardS(_shiftS == sizeof(S)*8),
    _wildcardT(_shiftT == sizeof(T)*8),
    _wildcardU(_shiftU == sizeof(U)*8),
    _wildcardV(_shiftV == sizeof(V)*8),
    _wildcardW(_shiftW == sizeof(W)*8),
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
  void insert(std::unique_ptr<PrefixRule<S, T, U, V, W>> ruleptr) {
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
      PrefixRule<S, T, U, V, W>* prev = _storage[index];
      PrefixRule<S, T, U, V, W>* next = prev->next;

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
  inline bool responsible(const std::unique_ptr<PrefixRule<S, T, U, V, W>>& ruleptr) const {
    // consider only first two fields which results in less tuples
    const uint32_t mask = 0xFFFFC000;
    return (ruleptr->tuplemap->bitwidthmap & mask) == (_tuplemap->bitwidthmap & mask);
  }

  /** Search for matching rule and returning its index, if available. */
  unsigned long int lookup(const Data5tpl::HeaderTuple& header) {
    uint32_t index = calcHash(header.addrSrc, header.addrDest) % _capacity;

    // iterate over list
    PrefixRule<S, T, U, V, W>* rule = _storage[index]; _memdummy_storage[index];
      
    while (rule != nullptr) {
      if (rule->match(header.addrSrc, 
          header.addrDest, 
          header.portSrc, 
          header.portDest, 
          header.protocol)) {

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
      PrefixRule<S, T, U, V, W>* rule = _storage[i];
      
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
      PrefixRule<S, T, U, V, W>* rule = _storage[i];
      
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
        PrefixRule<S, T, U, V, W>* next = _storage[i]->next;

        if (_storage[i]->index == index) {
          // set next rule as current rule in list
          delete _storage[i]; // remove this
          _storage[i] = next;
          --i; continue; // repeat loop for same i
        }
        else { // iterate over list, if more rules exist
          PrefixRule<S, T, U, V, W>* prev = _storage[i];
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
  inline bool operator<(const Map<S, T, U, V, W>& oth) const { return _minIndex < oth.getMinIndex(); }
};

/** Map for the common five tuple. */
typedef Map<uint32_t, uint32_t, uint16_t, uint16_t, uint8_t> Map5tpl;
typedef std::unique_ptr<Map5tpl> Map5ptr;

} // namespace DataTupleSpace5tpl

#endif

