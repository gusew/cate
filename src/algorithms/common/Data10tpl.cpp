#include <algorithms/common/Data10tpl.hpp>

bool Data10tpl::Rule::match(const HeaderTuple& tupl) {
    return _check1->match(tupl.v1) &&
      _check2->match(tupl.v2) &&
      _check3->match(tupl.v3) &&
      _check4->match(tupl.v4) &&
      _check5->match(tupl.v5) &&
      _check6->match(tupl.v6) &&
      _check7->match(tupl.v7) &&
      _check8->match(tupl.v8) &&
      _check9->match(tupl.v9) &&
      _check10->match(tupl.v10);
}

void Data10tpl::RuleSet::setRule(uint32_t index, std::unique_ptr<Rule> ruleptr) {
    if (index >= _size) throw "Index for setting Rule is outside bounds.";
    _rules[index] = std::move(ruleptr);
}

bool Data10tpl::RuleSet::match(const HeaderTuple& tupl, uint32_t& index) const {
  uint32_t i;

  for (i = 0; i < _size; ++i) // go through each rule
      if (_rules[i]->match(tupl)) break;

  if (i < _size) { // rule[i] matched
      index = i;
      return true;
  } else
      return false;
}

bool Data10tpl::RuleSet::swapRule(uint32_t index, std::unique_ptr<Data10tpl::Rule>& ruleptr) {
  if (index >= _size) return false; // invalid index given
  if (!_rules[index]) return false; // rule pointer is currently not set

  ruleptr.swap(_rules[index]);
  return true;
}

namespace Data10tpl {
std::ostream& operator<<(std::ostream& o, const HeaderTuple& tpl) { 
  o << "HeaderTuple (10tpl): v1= " << std::hex << tpl.v1 << ", ";
  o << "v2= " << std::hex << tpl.v2 << ", ";
  o << "v3= " << std::hex << tpl.v3 << ", ";
  o << "v4= " << std::hex << tpl.v4 << ", ";
  o << "v5= " << std::hex << tpl.v5 << ", ";
  o << "v6= " << std::hex << tpl.v6 << ", ";
  o << "v7= " << std::hex << tpl.v7 << ", ";
  o << "v8= " << std::hex << tpl.v8 << ", ";
  o << "v9= " << std::hex << tpl.v9 << ", ";
  o << "v10= " << std::hex << tpl.v10;

  return o; 
}

std::ostream& operator<< (std::ostream& o, const Data10tpl::Rule& rule) {
  o << "Rule (10tpl): ch1= " << *(rule._check1) << ", ";
  o << "ch2= " << *(rule._check2) << ", ";
  o << "ch3= " << *(rule._check3) << ", ";
  o << "ch4= " << *(rule._check4) << ", ";
  o << "ch5= " << *(rule._check5) << ", ";
  o << "ch6= " << *(rule._check6) << ", ";
  o << "ch7= " << *(rule._check7) << ", ";
  o << "ch8= " << *(rule._check8) << ", ";
  o << "ch9= " << *(rule._check9) << ", ";
  o << "ch10= " << *(rule._check10);

	return o;
}
} // namespace Data10tpl

