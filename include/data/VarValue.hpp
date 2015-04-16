#ifndef VAR_VALUE_INCLUDED
#define VAR_VALUE_INCLUDED

#include <gmpxx.h>

namespace Generic {

/** Use GMP library for multiprecision type with built-in math-operations. */
typedef mpz_class VarValue;

/** Due to internal signed representation of mpz_class, a proper negation is done with this function. */
VarValue negateVarValue(const VarValue& val);

} // namespace Generic

#endif

