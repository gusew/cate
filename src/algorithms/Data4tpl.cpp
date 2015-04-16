#include <algorithms/Data4tpl.hpp>

bool Data4tpl::Rule::match(const HeaderTuple& tupl) {
    return _check1->match(tupl.v1) && 
      _check2->match(tupl.v2) && 
      _check3->match(tupl.v3) && 
      _check4->match(tupl.v4);
}

void Data4tpl::RuleSet::setRule(uint32_t index, std::unique_ptr<Rule> ruleptr) {
    if (index >= _size) throw "Index for setting Rule is outside bounds.";
    _rules[index] = std::move(ruleptr);
}

bool Data4tpl::RuleSet::match(const HeaderTuple& tupl, uint32_t& index) const {
  uint32_t i;

  for (i = 0; i < _size; ++i) // go through each rule
      if (_rules[i]->match(tupl)) break;

  if (i < _size) { // rule[i] matched
      index = i;
      return true;
  } else
      return false;
}

bool Data4tpl::RuleSet::swapRule(uint32_t index, std::unique_ptr<Data4tpl::Rule>& ruleptr) {
  if (index >= _size) return false; // invalid index given
  if (!_rules[index]) return false; // rule pointer is currently not set

  ruleptr.swap(_rules[index]);
  return true;
}

namespace Data4tpl {
std::ostream& operator<<(std::ostream& o, const HeaderTuple& tpl) { 
  o << "HeaderTuple (4tpl): v1= " << std::hex << tpl.v1 << ", ";
  o << "v2= " << std::hex << tpl.v2 << ", ";
  o << "v3= " << std::hex << tpl.v3 << ", ";
  o << "v4= " << std::hex << tpl.v4 << std::endl;

  return o; 
}

std::ostream& operator<< (std::ostream& o, const Data4tpl::Rule& rule) {
  o << "Rule (4tpl): check1= " << *(rule._check1) << ", ";
  o << "check2= " << *(rule._check2) << ", ";
  o << "check3= " << *(rule._check3) << ", ";
  o << "check4= " << *(rule._check4) << std::endl;

	return o;
}
} // namespace Data4tpl

