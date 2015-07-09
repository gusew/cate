#ifndef DATA_5TPL_INCLUDED
#define DATA_5TPL_INCLUDED

#include <cstdint>
#include <memory>
#include <iostream>
#include <metering/memory/MemTrace.hpp>

namespace Data5tpl {

/**
 * A simple Tuple of values for 5tpl. For representing a packet header data set.
 */
struct HeaderTuple {
  uint32_t addrSrc;
  uint32_t addrDest;
  uint16_t portSrc;
  uint16_t portDest;
  uint8_t protocol;

  HeaderTuple(uint32_t addressSrc, uint32_t addressDest,
      uint16_t portnoSrc, uint16_t portnoDest,
      uint8_t ipProtocol) :
      addrSrc(addressSrc), addrDest(addressDest),
      portSrc(portnoSrc), portDest(portnoDest),
      protocol(ipProtocol) { }
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
 * A rule entry for an 5tpl-filter set.
 */
class Rule {
  std::unique_ptr<RuleAtom<uint32_t>> _addressSrc;
  std::unique_ptr<RuleAtom<uint32_t>> _addressDest;
  std::unique_ptr<RuleAtom<uint16_t>> _portSrc;
  std::unique_ptr<RuleAtom<uint16_t>> _portDest;
  std::unique_ptr<RuleAtom<uint8_t>> _protocol;

public:
  Rule(std::unique_ptr<RuleAtom<uint32_t>> addrSrc, std::unique_ptr<RuleAtom<uint32_t>> addrDest, std::unique_ptr<RuleAtom<uint16_t>> portSrc, std::unique_ptr<RuleAtom<uint16_t>> portDest, std::unique_ptr<RuleAtom<uint8_t>> protocol) :
      _addressSrc(std::move(addrSrc)), _addressDest(std::move(addrDest)),
      _portSrc(std::move(portSrc)), _portDest(std::move(portDest)),
      _protocol(std::move(protocol)) { }
  ~Rule() {}

  /**
   * Check if a given tuple is matched by this rule (and so by each rule atom).
   *
   * @param tupl given tuple to check if matched by rule
   * @return true, if tuple matched, otw. false
   */
  bool match(const HeaderTuple& tupl);

  // output to console
  friend std::ostream& operator<<(std::ostream& o, const Data5tpl::Rule& rule);
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

} // namespace Data5tpl
#endif
