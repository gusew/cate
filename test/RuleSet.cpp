#include <libunittest/all.hpp>
#include <generics/RuleSet.hpp>
#include <generics/RuleAtom.hpp>
#include <memory>

using namespace unittest::assertions;
using namespace Generic;

TEST(test_ruleset_isvalid)
{
	RuleSet ruleset;
	std::unique_ptr<Rule> rule1(new Rule);
	std::unique_ptr<RuleAtom> atom1(new RuleAtomExact((unsigned int)142));
	rule1->push_back(std::move(atom1));
	std::unique_ptr<Rule> rule2(new Rule);
	std::unique_ptr<RuleAtom> atom2(new RuleAtomRange((unsigned int)99, (unsigned int)256));
	rule2->push_back(std::move(atom2));
	std::unique_ptr<Rule> rule3(new Rule);
	std::unique_ptr<RuleAtom> atom3(new RuleAtomPrefix((unsigned int)0x4FFFFFFF, (unsigned int)0xFFFFFF00));
	rule3->push_back(std::move(atom3));
	ruleset.push_back(std::move(rule1));
	ruleset.push_back(std::move(rule2));
	ruleset.push_back(std::move(rule3));
	assert_true(ruleSetIsValid(ruleset), SPOT);
	assert_equal(ruleset.size(), (unsigned)3, SPOT);
}

TEST(test_ruleset_wrongarity)
{
	RuleSet ruleset;
	std::unique_ptr<Rule> rule1(new Rule);
	std::unique_ptr<RuleAtom> atom11(new RuleAtomExact((unsigned int)9));
	rule1->push_back(std::move(atom11));
	std::unique_ptr<Rule> rule2(new Rule);
	std::unique_ptr<RuleAtom> atom21(new RuleAtomExact((unsigned int)42));
	std::unique_ptr<RuleAtom> atom22(new RuleAtomExact((unsigned int)123));
	rule2->push_back(std::move(atom21));
	rule2->push_back(std::move(atom22));
	ruleset.push_back(std::move(rule1));
	ruleset.push_back(std::move(rule2));
	assert_false(ruleSetIsValid(ruleset), SPOT);
}

TEST(test_ruleset_empty)
{
	RuleSet ruleset;

	assert_true(ruleSetIsValid(ruleset), SPOT);
	assert_equal(ruleset.size(), (unsigned)0, SPOT);
}

TEST(test_ruleset_torange)
{
  VarValue resmin, resmax;
  
  RuleAtomExact exact1((unsigned int)0);
  exact1.toRange(resmin, resmax);
  assert_equal(resmin, 0, SPOT);
  assert_equal(resmax, 0, SPOT);

  RuleAtomExact exact2((unsigned int)142);
  exact2.toRange(resmin, resmax);
  assert_equal(resmin, 142, SPOT);
  assert_equal(resmax, 142, SPOT);

  VarValue longint1("0xABCDAFFE1234");
  RuleAtomExact exact3(longint1);
  exact3.toRange(resmin, resmax);
  assert_equal(resmin, longint1, SPOT);
  assert_equal(resmax, longint1, SPOT);

  RuleAtomRange range1((unsigned int)0, (unsigned int)0xFFFFFFFF);
  range1.toRange(resmin, resmax);
  assert_equal(resmin, 0, SPOT);
  assert_equal(resmax, 0xFFFFFFFF, SPOT);

  VarValue intval1("0xABC768");
  RuleAtomRange range2(intval1, longint1);
  range2.toRange(resmin, resmax);
  assert_equal(resmin, intval1, SPOT);
  assert_equal(resmax, longint1, SPOT);

  RuleAtomPrefix prefix1((unsigned int)0, (unsigned int)0xFFFFFF);
  prefix1.toRange(resmin, resmax);
  assert_equal(resmin, 0, SPOT);
  assert_equal(resmax, 0, SPOT);

  RuleAtomPrefix prefix2((unsigned int)0x17880, (unsigned int)0xFFF00);
  prefix2.toRange(resmin, resmax);
  assert_equal(resmin, 0x17800, SPOT);
  assert_equal(resmax, 0x178FF, SPOT);

  VarValue longprefix("0xABCDEF87654321"), longmask("0xFFFFF000000000"), longminval("0xABCDE000000000"), longmaxval("0xABCDEFFFFFFFFF");
  RuleAtomPrefix prefix3(longprefix, longmask);
  prefix3.toRange(resmin, resmax);
  assert_equal(resmin, longminval, SPOT);
  assert_equal(resmax, longmaxval, SPOT);

  // special cases: wildcard prefix
  RuleAtomPrefix prefixzero((unsigned int)0, (unsigned int)0);
  prefixzero.toRange(resmin, resmax);
  assert_equal(resmin, 0, SPOT);
  assert_equal(resmax, 0, SPOT); // defined for a wildcard-range

  RuleAtomPrefix prefixsth((unsigned int)0x1234567, (unsigned int)0);
  prefixsth.toRange(resmin, resmax);
  assert_equal(resmin, 0, SPOT);
  assert_equal(resmax, 0, SPOT); // defined for a wildcard-range
}

