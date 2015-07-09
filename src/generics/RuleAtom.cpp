#include <generics/RuleAtom.hpp>

void Generic::RuleAtomRange::toPrefixes(std::vector<std::tuple<VarValue, VarValue>>& result) const {

  VarValue r_min = minValue;
  VarValue r_max = maxValue;

  while (r_min <= r_max) { 
    for (unsigned int i = 0; i < (width+1); ++i) {
      const VarValue pot = (VarValue)1 << (i + 1);
      if (((r_min % pot) != 0) || ((r_min + pot - 1) > r_max)) {
        result.push_back(std::make_pair(r_min, (width-i)));
        r_min += (VarValue)1 << i;
        break;
      }
    }
  }
}

size_t Generic::RuleAtomPrefix::maskLength() const {
  size_t numbits = 0;
  
  for (size_t i = 0; i < width; ++i) {
    if (((mask >> i) & 0x1) == 1)
      ++numbits;
  }
  return numbits;
}
