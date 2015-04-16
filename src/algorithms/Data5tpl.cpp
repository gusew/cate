#include <algorithms/Data5tpl.hpp>

bool Data5tpl::Rule::match(const HeaderTuple& tupl) {
    return _addressSrc->match(tupl.addrSrc) &&
      _addressDest->match(tupl.addrDest) &&
      _portSrc->match(tupl.portSrc) &&
      _portDest->match(tupl.portDest) &&
      _protocol->match(tupl.protocol);
}

void Data5tpl::RuleSet::setRule(uint32_t index, std::unique_ptr<Rule> ruleptr) {
    if (index >= _size) throw "Index for setting Rule is outside bounds.";
    _rules[index] = std::move(ruleptr);
}

bool Data5tpl::RuleSet::match(const HeaderTuple& tupl, uint32_t& index) const {
  uint32_t i;

  for (i = 0; i < _size; ++i) // go through each rule
      if (_rules[i]->match(tupl)) break;

  if (i < _size) { // rule[i] matched
      index = i;
      return true;
  } else
      return false;
}

bool Data5tpl::RuleSet::swapRule(uint32_t index, std::unique_ptr<Data5tpl::Rule>& ruleptr) {
  if (index >= _size) return false; // invalid index given
  if (!_rules[index]) return false; // rule pointer is currently not set

  ruleptr.swap(_rules[index]);
  return true;
}

namespace Data5tpl {
std::ostream& operator<<(std::ostream& o, const HeaderTuple& tpl) { 
  o << "HeaderTuple (5tpl): addrSrc= " << std::hex << tpl.addrSrc << ", ";
  o << "addrDest= " << std::hex << tpl.addrDest << ", ";
  o << "portSrc= " << std::hex << tpl.portSrc << ", ";
  o << "portDest= " << std::hex << tpl.portDest << ", ";
  o << "protocol= " << std::hex << (uint16_t)tpl.protocol;

  return o; 
}

std::ostream& operator<< (std::ostream& o, const Data5tpl::Rule& rule) {
  o << "Rule (5tpl): addrSrc= " << *(rule._addressSrc) << ", ";
  o << "addrDest= " << *(rule._addressDest) << ", ";
  o << "portSrc= " << *(rule._portSrc) << ", ";
  o << "portDest= " << *(rule._portDest) << ", ";
  o << "protocol= " << *(rule._protocol);

	return o;
}
} // namespace Data5tpl

