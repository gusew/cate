#ifndef RULEATOM_INCLUDED
#define RULEATOM_INCLUDED

#include <generics/VarValue.hpp>
#include <tuple>

namespace Generic {

/** Abstract base type */
struct RuleAtom {
  enum Type { EXACT, RANGE, PREFIX };

	virtual ~RuleAtom() { }
	virtual Type getType() const = 0;
	virtual bool isWildcard() const = 0;
  /** Returns this atom converted in range representation. */
  virtual void toRange(VarValue& min, VarValue& max) const = 0;
  virtual void toPrefixes(std::vector<std::tuple<VarValue, VarValue>>& result) const = 0;
};

/** Exact matching of values */
struct RuleAtomExact : RuleAtom {
	VarValue value;
  size_t width;

	RuleAtomExact(const VarValue& val, const size_t w = 64) : value(val), width(w) { }
	RuleAtomExact(uint64_t val, const size_t w = 64) : value(val), width(w) { }
	RuleAtomExact(uint32_t val, const size_t w = 32) : value(val), width(w) { }
	RuleAtomExact(uint16_t val, const size_t w = 16) : value(val), width(w) { }
	RuleAtomExact(uint8_t val, const size_t w = 8) : value(val), width(w) { }
	RuleAtomExact(const std::string& val, const size_t w = 64) : value(val), width(w) { }
	RuleAtomExact(double val, const size_t w = 64) : value(val), width(w) { }

	~RuleAtomExact() override { }
  
  inline Type getType() const override { return RuleAtom::Type::EXACT; }
  inline bool isWildcard() const override { return false; }
  inline void toRange(VarValue& min, VarValue& max) const override { min = value; max = value; }
  inline void toPrefixes(std::vector<std::tuple<VarValue, VarValue>>& result) const override { result.push_back(std::make_tuple(value, width)); }
};

/** Matches, if value is between minValue and maxValue */
struct RuleAtomRange : RuleAtom {
	VarValue minValue;
	VarValue maxValue;
  size_t width;

	RuleAtomRange(const VarValue& min, const VarValue& max, const size_t w = 64) : minValue(min), maxValue(max), width(w) { }
	RuleAtomRange(uint64_t min, uint64_t max, const size_t w = 64) : minValue(min), maxValue(max), width(w) { }
	RuleAtomRange(uint32_t min, uint32_t max, const size_t w = 32) : minValue(min), maxValue(max), width(w) { }
	RuleAtomRange(uint16_t min, uint16_t max, const size_t w = 16) : minValue(min), maxValue(max), width(w) { }
	RuleAtomRange(uint8_t min, uint8_t max, const size_t w = 8) : minValue(min), maxValue(max), width(w) { }
	RuleAtomRange(const std::string& min, const std::string& max, const size_t w = 64) : minValue(min), maxValue(max), width(w) { }
	RuleAtomRange(double min, double max, const size_t w = 64) : minValue(min), maxValue(max), width(w) { }

	~RuleAtomRange() override { }
  
  inline Type getType() const override { return RuleAtom::Type::RANGE; }
  bool isWildcard() const override { return false; }
  inline void toRange(VarValue& min, VarValue& max) const override { min = minValue; max = maxValue; }
  void toPrefixes(std::vector<std::tuple<VarValue, VarValue>>& result) const override;
};

/** Given value matches the prefixValue in all lower bits */
struct RuleAtomPrefix : RuleAtom {
	VarValue prefix;
	VarValue mask;
  size_t width;

	RuleAtomPrefix(const VarValue& val, const VarValue& msk, const size_t w = 64) : prefix(val), mask(msk), width(w) { }
	RuleAtomPrefix(uint64_t val, uint64_t msk, const size_t w = 64) : prefix(val), mask(msk), width(w) { }
	RuleAtomPrefix(uint32_t val, uint32_t msk, const size_t w = 32) : prefix(val), mask(msk), width(w) { }
	RuleAtomPrefix(uint16_t val, uint16_t msk, const size_t w = 16) : prefix(val), mask(msk), width(w) { }
	RuleAtomPrefix(uint8_t val, uint8_t msk, const size_t w = 8) : prefix(val), mask(msk), width(w) { }
	RuleAtomPrefix(const std::string& val, const std::string& msk, const size_t w = 64) : prefix(val), mask(msk), width(w) { }
	RuleAtomPrefix(double val, double msk, const size_t w = 64) : prefix(val), mask(msk), width(w) { }
	
  ~RuleAtomPrefix() override { }
  
  inline Type getType() const override { return RuleAtom::Type::PREFIX; }
  inline bool isWildcard() const override { return mask == 0; }
  inline void toRange(VarValue& min, VarValue& max) const override { min = prefix & mask; max = min | negateVarValue(mask); }
  inline void toPrefixes(std::vector<std::tuple<VarValue, VarValue>>& result) const override { result.push_back(std::make_tuple(prefix, maskLength())); }

private:
  size_t maskLength() const;
};

} // namespace Generic

#endif
