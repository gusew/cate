#include <generics/VarValue.hpp>

namespace Generic {
Generic::VarValue negateVarValue(const Generic::VarValue& val) {
  using namespace Generic;

  VarValue signbitkiller(1);
  while (signbitkiller <= val) { // leftshift with padding 1s
    signbitkiller = (signbitkiller << 1) | 1; 
  }
  signbitkiller = signbitkiller >> 1; // one too much

  VarValue result = (~val) & signbitkiller;
  return result;
}

} // namespace Generic

