#include <algorithms/Data2tpl.hpp>

bool Data2tpl::Rule::match(const HeaderTuple& tupl) {
    return _addressSrc->match(tupl.addrSrc) &&
      _addressDest->match(tupl.addrDest);
}

void Data2tpl::RuleSet::setRule(uint32_t index, std::unique_ptr<Rule> ruleptr) {
    if (index >= _size) throw "Index for setting Rule is outside bounds.";
    _rules[index] = std::move(ruleptr);
}

bool Data2tpl::RuleSet::match(const HeaderTuple& tupl, uint32_t& index) const {
  uint32_t i;

  for (i = 0; i < _size; ++i) // go through each rule
      if (_rules[i]->match(tupl)) break;

  if (i < _size) { // rule[i] matched
      index = i;
      return true;
  } else
      return false;
}

bool Data2tpl::RuleSet::swapRule(uint32_t index, std::unique_ptr<Data2tpl::Rule>& ruleptr) {
  if (index >= _size) return false; // invalid index given
  if (!_rules[index]) return false; // rule pointer is currently not set

  ruleptr.swap(_rules[index]);
  return true;
}

namespace Data2tpl {
std::ostream& operator<<(std::ostream& o, const HeaderTuple& tpl) { 
  o << "HeaderTuple (2tpl): addrSrc= " << std::hex << tpl.addrSrc << ", ";
  o << "addrDest= " << std::hex << tpl.addrDest << ", ";

  return o; 
}

std::ostream& operator<< (std::ostream& o, const Data2tpl::Rule& rule) {
  o << "Rule (2tpl): addrSrc= " << *(rule._addressSrc) << ", ";
  o << "addrDest= " << *(rule._addressDest) << ", ";

	return o;
}
} // namespace Data2tpl

