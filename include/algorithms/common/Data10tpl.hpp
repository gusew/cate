#ifndef DATA_10TPL_INCLUDED
#define DATA_10TPL_INCLUDED

#include <cstdint>
#include <memory>
#include <iostream>
#include <memory/MemTrace.hpp>

namespace Data10tpl {

/**
 * A simple Tuple of values for 10tpl. For representing a packet header data set.
 */
struct HeaderTuple {
  uint32_t v1;
  uint32_t v2;
  uint32_t v3;
  uint32_t v4;
  uint32_t v5;
  uint32_t v6;
  uint32_t v7;
  uint32_t v8;
  uint32_t v9;
  uint32_t v10;

  HeaderTuple(uint32_t val1, 
    uint32_t val2,
    uint32_t val3,
    uint32_t val4,
    uint32_t val5,
    uint32_t val6,
    uint32_t val7,
    uint32_t val8,
    uint32_t val9,
    uint32_t val10) :
    v1(val1),
    v2(val2),
    v3(val3),
    v4(val4),
    v5(val5),
    v6(val6),
    v7(val7),
    v8(val8),
    v9(val9),
    v10(val10)
  {}

  ~HeaderTuple() { }

  // output to console
  friend std::ostream& operator<<(std::ostream& o, const HeaderTuple& tpl);
};

/**
 * Abstract base class for a part (or atom) of a specific rule.
 */
template <typename T>
struct RuleAtom {
  virtual ~RuleAtom() { };

  /**
   * Is called to check if a given value matches with this rule atom.
   *
   * @param val given value to check if matches
   * @return true, if value matches, otw. false
   */
	virtual bool match(const T val) const = 0;

protected:
  // is overridden by sub classes to output their content
  virtual void print(std::ostream& o) const = 0;

public:
  // output to console
  inline friend std::ostream& operator<<(std::ostream& o, const RuleAtom& ruleAtom) { ruleAtom.print(o); return o; }
};

/**
 * Specific implementation of a rule atom for exact matches of values.
 */
template <typename T>
class RuleAtomExact : public RuleAtom<T>{
  Memory::MemTrace<T> _value;

public:
  RuleAtomExact(const T val) : _value(val) { }
  ~RuleAtomExact() override { };
	inline bool match(const T val) const override { return _value == val; }

protected:
  // output content to console
  void print(std::ostream& o) const override {
    o << "Exact[" << std::hex << (sizeof(T)>1 ? (T)_value : (uint16_t)_value) << "]";
  }
};

/**
 * Specific implementation of a rule atom for range matches of values.
 */
template <typename T>
class RuleAtomRange : public RuleAtom<T>{
  Memory::MemTrace<T> _minValue;
	Memory::MemTrace<T> _maxValue;

public:
	RuleAtomRange(const T min, const T max) : _minValue(min), _maxValue(max) { }
  ~RuleAtomRange() override { };
	inline bool match(const T val) const override { return val >= _minValue && val <= _maxValue; }

protected:
  // output content to console
  void print(std::ostream& o) const override {
    o << "Range[" << std::hex << (sizeof(T)>1 ? (T)_minValue : (uint16_t)_minValue) << "..." << std::hex << (sizeof(T)>1 ? (T)_maxValue : (uint16_t)_maxValue) << "]";
  }
};

/**
 * Specific implementation of a rule atom for prefix matches of values.
 */
template <typename T>
class RuleAtomPrefix : public RuleAtom<T>{
  Memory::MemTrace<T> _prefix;
	Memory::MemTrace<T> _mask;

public:
	RuleAtomPrefix(const T val, const T mask) : _prefix(val), _mask(mask) { }
  ~RuleAtomPrefix() override { };
	inline bool match(const T val) const override { return (val & _mask) == (_mask & _prefix); }

protected:
  // output content to console
  void print(std::ostream& o) const override {
    o << "Prefix[" << std::hex << (sizeof(T)>1 ? (T)_prefix : (uint16_t)_prefix) << "/mask:" << std::hex << (sizeof(T)>1 ? (T)_mask : (uint16_t)_mask) << "]";
  }
};

/**
 * A rule entry for an 10tpl-filter set.
 */
class Rule {
  std::unique_ptr<RuleAtom<uint32_t>> _check1;
  std::unique_ptr<RuleAtom<uint32_t>> _check2;
  std::unique_ptr<RuleAtom<uint32_t>> _check3;
  std::unique_ptr<RuleAtom<uint32_t>> _check4;
  std::unique_ptr<RuleAtom<uint32_t>> _check5;
  std::unique_ptr<RuleAtom<uint32_t>> _check6;
  std::unique_ptr<RuleAtom<uint32_t>> _check7;
  std::unique_ptr<RuleAtom<uint32_t>> _check8;
  std::unique_ptr<RuleAtom<uint32_t>> _check9;
  std::unique_ptr<RuleAtom<uint32_t>> _check10;

public:
  Rule(std::unique_ptr<RuleAtom<uint32_t>> c1, 
    std::unique_ptr<RuleAtom<uint32_t>> c2, 
    std::unique_ptr<RuleAtom<uint32_t>> c3, 
    std::unique_ptr<RuleAtom<uint32_t>> c4, 
    std::unique_ptr<RuleAtom<uint32_t>> c5, 
    std::unique_ptr<RuleAtom<uint32_t>> c6, 
    std::unique_ptr<RuleAtom<uint32_t>> c7, 
    std::unique_ptr<RuleAtom<uint32_t>> c8, 
    std::unique_ptr<RuleAtom<uint32_t>> c9, 
    std::unique_ptr<RuleAtom<uint32_t>> c10) :
    _check1(std::move(c1)),
    _check2(std::move(c2)),
    _check3(std::move(c3)),
    _check4(std::move(c4)),
    _check5(std::move(c5)),
    _check6(std::move(c6)),
    _check7(std::move(c7)),
    _check8(std::move(c8)),
    _check9(std::move(c9)),
    _check10(std::move(c10))
  {}

  ~Rule() {}

  /**
   * Check if a given tuple is matched by this rule (and so by each rule atom).
   *
   * @param tupl given tuple to check if matched by rule
   * @return true, if tuple matched, otw. false
   */
  bool match(const HeaderTuple& tupl);

  // output to console
  friend std::ostream& operator<<(std::ostream& o, const Data10tpl::Rule& rule);
};

/**
 * A simple collection of multiple rules in an array. Header tuples can be matched
 * by the ruleset and the index of the matching rule will be returned.
 */
class RuleSet {
  std::unique_ptr<std::unique_ptr<Rule>[]> _rules; // array of pointer to each rule
  uint32_t _size;

public:
  RuleSet(uint32_t initSize) : _rules(new std::unique_ptr<Rule>[initSize]), _size(initSize) { }
  ~RuleSet() {}

  /**
   * Set a rule-pointer inside the rule set (sink for rules).
   *
   * @param index array index of rule-to-set in set
   * @param ruleptr pointer to rule object
   */
  void setRule(uint32_t index, std::unique_ptr<Rule> ruleptr);

  /**
   * Test a header data set against each rule in ruleset and return index of matching rule.
   *
   * @param tupl given tuple to check if matched by one of the rules
   * @param index reference to variable where result-index of a rule should be stored, if matched
   * @return true, if tuple matched by one of the rules, otw. false
   */
  bool match(const HeaderTuple& tupl, uint32_t& index) const;

  /** Return the amount of rules inside this rule set. */
  inline uint32_t getSize() const { return _size; }

  /**
   * Moves the responsibility for a rule-pointer from this rule set
   * to another rule set. This rule set becomes invalid, as at least
   * the moved rule cannot be accessed any longer.
   *
   * @param index position of rule in array of pointers
   * @param ruleptr new rule pointer to be set with the moved rule
   * @return true, if successful, otw. false
   */
  bool swapRule(uint32_t index, std::unique_ptr<Rule>& ruleptr);
};

} // namespace Data10tpl
#endif
