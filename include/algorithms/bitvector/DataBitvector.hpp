#ifndef DATA_BITVECTOR_INCLUDED
#define DATA_BITVECTOR_INCLUDED

#include <memory>
#include <limits>
#include <map>
#include <forward_list>
#include <metering/memory/MemTrace.hpp>

namespace DataBitvector {

/** Concrete type of storage for multiple bits. */
typedef unsigned int StorageT;
typedef Memory::MemTrace<StorageT> MemStorageT;

/**
 * Holds each bit of the vector and provides functions for
 * operating on the vector, like retrieving the index of the first
 * set bit and the logical AND-operation.
 */
class Bitvector {
  /** Size of the vector in bits. */
  unsigned int _size;
  /** Size of the storage array in array-elements. */
  unsigned int _storageSize;

  /** container of variable size for all bits */
  std::unique_ptr<MemStorageT[]> _storage;

  /** Get machine dependent size of a single storage object. */
  inline unsigned int _getSizeStorageT() const { return sizeof(StorageT) * 8; }

  /** Calculate the required amount of storage-array elements for given amount of bits. */
  inline unsigned int _calcStorageSize(unsigned int bitsize) { return bitsize/_getSizeStorageT() + (bitsize % _getSizeStorageT() == 0 ? 0 : 1); }

  /** 
   * Returns the storage value where the bit of given position is stored. 
   *
   * @param position position of relevant bit in bitvector
   * @param bitpos to return the bit-position inside returned storage
   * @returns storage value with contained bit at bit-position 'bitpos'
   */
  MemStorageT& _getStorageReference(unsigned int position, unsigned int& bitpos) const;

protected:

  /** Returns the size of this bitvector in amounts of storages. */
  inline unsigned int getStorageSize() const { return _storageSize; }

  /** Get a storage object with multiple bits. */
  StorageT getStorage(unsigned int position) const;
  
public:

  Bitvector() = delete;
  Bitvector(unsigned int size);
  Bitvector(const Bitvector& other); // copy c'tor
  ~Bitvector() {}

  Bitvector& operator=(const Bitvector& other); // copy assignment

  /** overload unary AND-operator */
  Bitvector& operator&=(const Bitvector& rhs);
  /** overload unary OR-operator */
  Bitvector& operator|=(const Bitvector& rhs);
  
  /** Returns size of this bitvector in total amount of bits. */
  inline unsigned int getSize() const { return _size; }

  /** Set a bit at given bit position to 1. */
  void setBit(unsigned int position);

  /** Get the Boolean value of a bit at the given position. */
  bool getBit(unsigned int position);

  /** Get index of first set bit in vector. */
  unsigned long int getFirstSetBit() const;
};

/** overload binary AND-operator */
Bitvector operator&(const Bitvector& lhs, const Bitvector& rhs);
/** overload binary OR-operator */
Bitvector operator|(const Bitvector& lhs, const Bitvector& rhs);

/** Used for returning that no bits are set in a bitvector. */
inline unsigned long int noBitSetInBitvector() { return std::numeric_limits<unsigned long int>::max(); }

/** Represents a simple numerical range with detection of overlapping. */
template <typename T>
struct Range {
  T min;
  T max;

  Range(const T& minVal, const T& maxVal) : min(minVal), max(maxVal) {}
  Range(T&& minVal, T&& maxVal) : min(minVal), max(maxVal) {}
  Range(const Range<T>& other) : min(other.min), max(other.max) {}

  /** Overload less-operator for sorting of ranges */
  inline bool operator<(const Range<T>& rhs) const {
    return (min < rhs.min || (min == rhs.min && max < rhs.max));
  }
  /** Overload equals-operator */
  inline bool operator==(const Range<T>& rhs) const {
    return (min == rhs.min && max == rhs.max);
  }

  /** Returns true, if overlapping between this range and another exists. */
  inline bool overlaps(const Range<T>& rhs) const {
    return (min <= rhs.min && max >= rhs.min) || (rhs.min <= min && rhs.max >= min); 
  }
};

/** Represents a five-tuple rule with ranges for the rule checks. */
template <typename S, typename T, typename U, typename V, typename W>
struct Rule5tpl {
  Range<S> check1;
  Range<T> check2;
  Range<U> check3;
  Range<V> check4;
  Range<W> check5;

  Rule5tpl(const S& min1, const S& max1, 
    const T& min2, const T& max2,
    const U& min3, const U& max3,
    const V& min4, const V& max4,
    const W& min5, const W& max5) : 
      check1(min1, max1), check2(min2, max2), check3(min3, max3),
      check4(min4, max4), check5(min5, max5) {}
};

/** Represents a two-tuple rule with ranges for the rule checks. */
template <typename S, typename T>
struct Rule2tpl {
  Range<S> check1;
  Range<T> check2;

  Rule2tpl(const S& min1, const S& max1, 
    const T& min2, const T& max2) : 
      check1(min1, max1), check2(min2, max2) {}
};

/** Represents a 4-tuple rule with ranges for the rule checks. */
template <typename S>
struct Rule4tpl {
  Range<S> check1;
  Range<S> check2;
  Range<S> check3;
  Range<S> check4;

  Rule4tpl(const S& min1, const S& max1, 
    const S& min2, const S& max2,
    const S& min3, const S& max3,
    const S& min4, const S& max4) : 
      check1(min1, max1), 
      check2(min2, max2),
      check3(min3, max3),
      check4(min4, max4)
  {}
};

/** Represents a 6-tuple rule with ranges for the rule checks. */
template <typename S>
struct Rule6tpl {
  Range<S> check1;
  Range<S> check2;
  Range<S> check3;
  Range<S> check4;
  Range<S> check5;
  Range<S> check6;

  Rule6tpl(const S& min1, const S& max1, 
    const S& min2, const S& max2,
    const S& min3, const S& max3,
    const S& min4, const S& max4,
    const S& min5, const S& max5,
    const S& min6, const S& max6) : 
      check1(min1, max1), 
      check2(min2, max2),
      check3(min3, max3),
      check4(min4, max4),
      check5(min5, max5),
      check6(min6, max6)
  {}
};

/** Represents a 8-tuple rule with ranges for the rule checks. */
template <typename S>
struct Rule8tpl {
  Range<S> check1;
  Range<S> check2;
  Range<S> check3;
  Range<S> check4;
  Range<S> check5;
  Range<S> check6;
  Range<S> check7;
  Range<S> check8;

  Rule8tpl(const S& min1, const S& max1, 
    const S& min2, const S& max2,
    const S& min3, const S& max3,
    const S& min4, const S& max4,
    const S& min5, const S& max5,
    const S& min6, const S& max6,
    const S& min7, const S& max7,
    const S& min8, const S& max8) : 
      check1(min1, max1), 
      check2(min2, max2),
      check3(min3, max3),
      check4(min4, max4),
      check5(min5, max5),
      check6(min6, max6),
      check7(min7, max7),
      check8(min8, max8)
  {}
};


/** Represents a ten-tuple rule with ranges for the rule checks. */
template <typename S>
struct Rule10tpl {
  Range<S> check1;
  Range<S> check2;
  Range<S> check3;
  Range<S> check4;
  Range<S> check5;
  Range<S> check6;
  Range<S> check7;
  Range<S> check8;
  Range<S> check9;
  Range<S> check10;

  Rule10tpl(const S& min1, const S& max1, 
    const S& min2, const S& max2,
    const S& min3, const S& max3,
    const S& min4, const S& max4,
    const S& min5, const S& max5,
    const S& min6, const S& max6,
    const S& min7, const S& max7,
    const S& min8, const S& max8,
    const S& min9, const S& max9,
    const S& min10, const S& max10) : 
      check1(min1, max1), 
      check2(min2, max2), 
      check3(min3, max3),
      check4(min4, max4), 
      check5(min5, max5),
      check6(min6, max6), 
      check7(min7, max7), 
      check8(min8, max8),
      check9(min9, max9), 
      check10(min10, max10)     
  {}
};


/** Functional object for overlapping-detection. */
template <typename T>
struct RangeLessOverlapping {
  inline bool operator()(const Range<T>& lhs, const Range<T>& rhs) const { return (lhs.overlaps(rhs) ? false : lhs.min < rhs.min); }
};


/** Organizes all ranges in one dimension for searching and retrieving bitvectors. */
template <typename T>
class DimensionRanges {
  /** Holds all available ranges and their associated bitvectors. */
  std::multimap<Range<T>, Bitvector, RangeLessOverlapping<T>> _ranges;

  /** Size of each contained bitvector. */
  unsigned int _bitvectorSize;
  
  /** Minimum and maximum values for all ranges. */
  T _domainMin;
  T _domainMax;

  /** Insert range-instance over whole defined domain. */
  void _initRange() {
    Range<T> complete(_domainMin, _domainMax);
    Bitvector zeros(_bitvectorSize);
    _ranges.insert(std::make_pair(complete, zeros)); 
  }

public:
  
  /** Provides not enough information to create feasible instance. */
  DimensionRanges() = delete;

  /**
   * Create a representation for all ranges in one dimension. 
   *
   * @param bvSizes total size of all bitvectors in bits
   * @param min minimum value for whole range-domain
   * @param max maximum value for whole range-domain
   */
  DimensionRanges(unsigned int bvSizes, T min, T max) : _ranges(), _bitvectorSize(bvSizes), _domainMin(min), _domainMax(max) { _initRange(); }

  /** Removes all existing ranges in container and initializes it again. */
  void reset() {
    _ranges.clear();
    _initRange();
  }

  /** Returns the size of the internal bitvectors. */
  inline unsigned int getSize() { return _bitvectorSize; }

  /**
   * Add a given range with the bit-position to the container. Before
   * the range is inserted, possible overlapping ranges are merged
   * with new range by 
   *
   * @param min minimum value of range to add
   * @param max maximum value of range to add
   * @param bvPos position in bitvector, where bits will be set to 1s
   */
  void addRange(T min, T max, unsigned int bvPos) {
    Range<T> key(min, max);

    // lists to store new ranges and the ones to delete temporarily 
    // (otw. iterators are changed when manipulating _ranges directly)
    std::forward_list<Range<T>> rangesToRemove;
    std::forward_list<std::pair<Range<T>, Bitvector>> rangesToInsert; 

    auto resPair = _ranges.equal_range(key);
    
    for (auto iter = resPair.first; iter != resPair.second; ++iter) {
      Range<T> found(iter->first);
      if (found == key) { 
        // both ranges identical
        iter->second.setBit(bvPos);
      }
      else if (found.min >= key.min && found.max <= key.max) { 
        // found lies completely in new range
        iter->second.setBit(bvPos);
      }
      else {
        // new ranges have to be created
        Bitvector foundBv(iter->second);

        // remove key-range that was found
        rangesToRemove.push_front(found);

        // check if region before overlapping exists
        if (found.min < key.min) {
          // create a new range for this non-overlapping region
          Range<T> preRange(found.min, key.min - 1);
          rangesToInsert.push_front(std::make_pair(preRange, foundBv));
        } 
        
        // check if region after overlapping exists
        if (key.max < found.max) {
          // create a new range for this non-overlapping region
          Range<T> postRange(key.max + 1, found.max);
          rangesToInsert.push_front(std::make_pair(postRange, foundBv));
        }

        // handle overlapping part
        foundBv.setBit(bvPos); // set bitvector at given position
        T lowerLimit = (key.min > found.min ? key.min : found.min);
        T upperLimit = (key.max < found.max ? key.max : found.max);

        Range<T> overlapRange(lowerLimit, upperLimit);
        rangesToInsert.push_front(std::make_pair(overlapRange, foundBv));
      }
    }

    // remove all cached ranges now from _ranges
    for (auto iter(rangesToRemove.begin()); iter != rangesToRemove.end(); ++iter) {
      _ranges.erase(*iter);
    }

    // insert all cached ranges now into _ranges
    for (auto iter(rangesToInsert.begin()); iter != rangesToInsert.end(); ++iter) {
      _ranges.insert(*iter);
    }
  }

  /** 
   * Search for a matching range with given value and fetch the 
   * associated bitvector as a copy.
   *
   * @param key a range, which might lie inside one of the ranges
   * @return copy of the associated bitvector or a vector with zeros, if not found
   */
  const Bitvector& search(const Range<T>& key) const {
    if (key.min < _domainMin || key.max > _domainMax)
      throw "DimensionRanges: Invalid value for searching was given!";

    // test for not more than one hit 
    //if (_ranges.count(key) > 1) 
    //  throw "DimensionRanges: There exist overlapping regions in ranges container!";
    
    auto result = _ranges.find(key);
    if (result != _ranges.end()) 
      return result->second;
    else 
      throw "No matching range was found";
  }
};

} // namespace DataBitvector

#endif

