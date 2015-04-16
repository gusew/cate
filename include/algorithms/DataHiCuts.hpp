#ifndef DATAHICUTS_INCLUDED
#define DATAHICUTS_INCLUDED

#include <memory>
#include <forward_list>
#include <vector>
#include <memory/MemTrace.hpp>

namespace DataHiCuts {

/** For tuning and customizing of the HiCuts algorithm. */
struct Parameters {
  unsigned int binth;
  double spfac;
  unsigned int totalRules;

  Parameters() : binth(2), spfac(1.0), totalRules(0) {}
  Parameters(unsigned int b, double fac, unsigned int ruleCnt) : binth(b), spfac(fac), totalRules(ruleCnt) {}
};

/** Represents a simple numerical range with detection of overlapping. */
template <typename T>
struct Range {
  Memory::MemTrace<T> min;
  Memory::MemTrace<T> max;

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

  inline void extend(const Range<T>& rhs) {
    if (rhs.min < min) min = rhs.min;
    if (rhs.max > max) max = rhs.max;
  }

  inline bool match(const T& val) const { return val >= min && val <= max; }

  inline double size() const { return (double)(max - min) + 1.0; }

  /**
   * Returns true, if this range is completely overlapped by other range in
   * a given geometrical box, specified by 'box'-range.
   *
   * @param oth range, which could potentially overlap this one
   * @param box geometrical limits for which inside-criteria is tested
   * @return true, if this range lies completely inside 'oth' range
   */
  inline bool inside(const Range<T>& oth, const Range<T>& box) const {
    return (oth.min <= (min > box.min ? min : box.min)) && (oth.max >= (max < box.max ? max : box.max));
  }
};

} // namespace DataHiCuts

#endif

