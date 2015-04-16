#ifndef RULE_INCLUDED
#define RULE_INCLUDED

#include <vector>
#include <memory>
#include <data/RuleAtom.hpp>

namespace Generic {

/** Represents a Rule with a certain amount of rule atoms. */
typedef std::vector<std::unique_ptr<RuleAtom>> Rule;

} // namespace Generic

#endif
