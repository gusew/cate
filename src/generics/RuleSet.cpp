#include <generics/RuleSet.hpp>

bool Generic::ruleSetIsValid(RuleSet& rs) {
	// check, if amount of Atoms per Rule is consistent
	int numAtoms = 0;
	int ruleArity = 0;

	for (RuleSet::iterator iter(rs.begin()); iter != rs.end(); ++iter) {
		Rule* r = iter->get();
		ruleArity = r->size();

		if (numAtoms == 0) // first Rule
			numAtoms = ruleArity;
		else // compare arities
			if (ruleArity != numAtoms) return false;
	}

	return true;
}

