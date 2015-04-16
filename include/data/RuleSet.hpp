#ifndef RULESET_INCLUDED
#define RULESET_INCLUDED

#include <vector>
#include <memory>
#include <data/Rule.hpp>
#include <limits>

namespace Generic {

/** Represents a collection of Rules (Wrapper around vector). */
typedef std::vector<std::unique_ptr<Rule>> RuleSet;

/** Is used for defining a header, which doesn't match a rule. */
typedef RuleSet::size_type RuleSetSize;
    
constexpr RuleSetSize noRuleIsMatching() { return std::numeric_limits<Generic::RuleSetSize>::max(); }

/** A collection of matching indices of rules created while filtering headers. */
typedef std::vector<RuleSetSize> RuleIndexSet;

/** Checks a given ruleset for validity */
bool ruleSetIsValid(RuleSet& rs);

} // namespace Generic
#endif

