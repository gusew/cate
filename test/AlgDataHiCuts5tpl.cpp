#include <libunittest/all.hpp>
#include <algorithms/hicuts/DataHiCuts5tpl.hpp>
#include <test/AlgTestFixtures.hpp>

using namespace unittest::assertions;
using namespace DataHiCuts5tpl;
using namespace DataHiCuts;

TEST(test_datahicuts_range_overlaps)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Range<unsigned int> r1(17, 30);
  Range<unsigned int> r2(30, 20000);
  Range<unsigned int> r3(0, 0xFFFFFFFF);
  Range<unsigned int> r4(2, 16);
  Range<unsigned int> r5(2, 16);

  assert_true(r1.overlaps(r2), SPOT);
  assert_true(r2.overlaps(r1), SPOT);

  assert_true(r1.overlaps(r3), SPOT);
  assert_true(r3.overlaps(r1), SPOT);

  assert_false(r1.overlaps(r4), SPOT);
  assert_false(r1.overlaps(r5), SPOT);
  assert_false(r4.overlaps(r1), SPOT);
  assert_false(r5.overlaps(r1), SPOT);

  assert_true(r2.overlaps(r3), SPOT);
  assert_true(r3.overlaps(r2), SPOT);

  assert_false(r2.overlaps(r4), SPOT);
  assert_false(r2.overlaps(r5), SPOT);
  assert_false(r4.overlaps(r2), SPOT);
  assert_false(r5.overlaps(r2), SPOT);

  assert_true(r3.overlaps(r4), SPOT);
  assert_true(r3.overlaps(r5), SPOT);
  assert_true(r4.overlaps(r3), SPOT);
  assert_true(r5.overlaps(r3), SPOT);

  assert_true(r4.overlaps(r5), SPOT);
  assert_true(r5.overlaps(r4), SPOT);
}

TEST(test_datahicuts_range_extend)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Range<unsigned int> r1(17, 30);
  Range<unsigned int> r2(30, 20000);
  Range<unsigned int> r3(0, 0xFFFFFFFF);
  Range<unsigned int> r4(2, 16);
  Range<unsigned int> r5(2, 16);

  r1.extend(r2);
  assert_equal(r1.min, (unsigned)17, SPOT);
  assert_equal(r1.max, (unsigned)20000, SPOT);
  // no side effects
  assert_equal(r2.min, (unsigned)30, SPOT);
  assert_equal(r2.max, (unsigned)20000, SPOT);

  r2.extend(r3);
  assert_equal(r2.min, (unsigned)0, SPOT);
  assert_equal(r2.max, (unsigned)0xFFFFFFFF, SPOT);

  r4.extend(r5);
  assert_equal(r4.min, (unsigned)2, SPOT);
  assert_equal(r4.max, (unsigned)16, SPOT);

  r5.extend(r1);
  assert_equal(r5.min, (unsigned)2, SPOT);
  assert_equal(r5.max, (unsigned)20000, SPOT);
}

TEST(test_datahicuts_range_match)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Range<unsigned int> r1(17, 30);
  Range<unsigned int> r2(30, 20000);
  Range<unsigned int> r3(0, 0xFFFFFFFF);

  assert_true(r1.match(17), SPOT);
  assert_true(r1.match(22), SPOT);
  assert_true(r1.match(30), SPOT);
  assert_true(r2.match(31), SPOT);
  assert_true(r2.match(19999), SPOT);
  assert_true(r2.match(20000), SPOT);

  assert_true(r3.match(42), SPOT);
  assert_true(r3.match(0), SPOT);
  assert_true(r3.match(0xFFFFFFFF), SPOT);

  assert_false(r1.match(16), SPOT);
  assert_false(r1.match(31), SPOT);
  assert_false(r1.match(12345), SPOT);

  assert_false(r2.match(17), SPOT);
  assert_false(r2.match(30000), SPOT);
}

TEST(test_datahicuts_range_size)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Range<unsigned int> r1(2, 5);
  Range<unsigned int> r2(30, 20000);
  Range<unsigned int> r3(0, 0xFFFFFFFF);

  assert_equal(r1.size(), 4.0, SPOT);
  assert_equal(r2.size(), 19971.0, SPOT);
  assert_equal(r3.size(), ((double)0xFFFFFFFF) + 1.0, SPOT); 
}

TEST(test_datahicuts_rangerule_overlaps)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Rule5tpl rule1(42, 0, 0xFFFF, 10000, 40000, 80, 1024, 80, 1024, 6, 6);
  Range<uint32_t> r1(17, 30);
  Range<uint32_t> r2(12000, 14000);
  Range<uint16_t> r3(100, 1000);
  Range<uint16_t> r4(500, 500);
  Range<uint8_t> r5(2, 10);
  assert_true(rule1.overlaps(r1, r2, r3, r4, r5), SPOT);

  r2.min = 9000;
  r2.max = 9999;
  assert_false(rule1.overlaps(r1, r2, r3, r4, r5), SPOT);
  
  r2.max = 10000;
  assert_true(rule1.overlaps(r1, r2, r3, r4, r5), SPOT);

  r1.max = 0x10000;
  r1.min = 0x10000;
  assert_false(rule1.overlaps(r1, r2, r3, r4, r5), SPOT);
  
  r1.min = 0xFFFF;
  assert_true(rule1.overlaps(r1, r2, r3, r4, r5), SPOT);

  r5.max = 5;
  assert_false(rule1.overlaps(r1, r2, r3, r4, r5), SPOT);

  r5.max = 6;
  assert_true(rule1.overlaps(r1, r2, r3, r4, r5), SPOT);
}

TEST(test_datahicuts_rangerule_match)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Rule5tpl rule1(42, 0, 0xFFFF, 10000, 40000, 80, 1024, 80, 1024, 6, 6);

  assert_true(rule1.match(0, 10000, 80, 80, 6), SPOT);
  assert_true(rule1.match(0xABC, 22222, 512, 256, 6), SPOT);
  assert_true(rule1.match(0xFFFF, 40000, 1024, 1024, 6), SPOT);

  assert_false(rule1.match(0x10000, 33333, 500, 128, 6), SPOT);
  assert_false(rule1.match(100, 40001, 500, 128, 6), SPOT);
  assert_false(rule1.match(100, 20001, 79, 128, 6), SPOT);
  assert_false(rule1.match(100, 20001, 81, 1025, 6), SPOT);
  assert_false(rule1.match(100, 20001, 81, 1023, 5), SPOT);
  assert_false(rule1.match(100, 20001, 81, 1023, 7), SPOT);
}

TEST(test_datahicuts_countdisjunct)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  std::shared_ptr<Rule5tpl> rule1 = std::make_shared<Rule5tpl>(0, 
    0, 0xFFFF, // disjunct with r3, r5
    0, 100, // disjunct with r2, r3, r4, r5
    80, 1024, // r2, r3
    0, 0, 6, 6); // r2, r3, r4, r5
  std::shared_ptr<Rule5tpl> rule2 = std::make_shared<Rule5tpl>(1, 
    0, 100, // r3, r5
    1000, 2000, // r3, r4, r5
    2000, 4000, // r4, r5
    17, 17, 6, 6); // r3, r4, r5
  std::shared_ptr<Rule5tpl> rule3 = std::make_shared<Rule5tpl>(2, 
    0x10000, 0x10000, // r4, r5
    3000, 10000, // r4, r5
    1500, 3000, // r5
    1000, 1000, 6, 6); // r4, r5
  std::shared_ptr<Rule5tpl> rule4 = std::make_shared<Rule5tpl>(3, 
    24, 0xFF, // r5
    20000, 40000, // -
    1000, 1500, // r5
    18181, 18181, 6, 6); // r5
  std::shared_ptr<Rule5tpl> rule5 = std::make_shared<Rule5tpl>(4, 
    0x10010, 0xFFFFFFFF,
    30000, 0xFFFF, 
    400, 428, 
    44444, 44444, 6, 6);

  RulesVector rules;
  rules.push_back(rule1);
  rules.push_back(rule2);
  rules.push_back(rule3);
  rules.push_back(rule4);
  rules.push_back(rule5);

  std::vector<unsigned int> results;
  countDisjunctPairs(rules, results);

  assert_equal(results.size(), (unsigned)5, SPOT);
  assert_equal(results[0], (unsigned)7, SPOT);
  assert_equal(results[1], (unsigned)9, SPOT);
  assert_equal(results[2], (unsigned)6, SPOT);
  assert_equal(results[3], (unsigned)10, SPOT);
  assert_equal(results[4], (unsigned)0, SPOT);
}

TEST(test_datahicuts_trienode_setrules)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  RulesVector rules;
  std::shared_ptr<Rule5tpl> r1 = std::make_shared<Rule5tpl>(0, 0, 20000, 0xFFFF, 0xFFFF, 80, 256, 128, 512, 6, 6);
  std::shared_ptr<Rule5tpl> r2 = std::make_shared<Rule5tpl>(1, 0, 10000, 0xFFFF, 0xFFFF, 100, 200, 42, 10000, 6, 12);
  std::shared_ptr<Rule5tpl> r3 = std::make_shared<Rule5tpl>(2, 0, 15000, 0xFFFF, 0xFFFF, 0, 1024, 100, 200, 6, 6);
  std::shared_ptr<Rule5tpl> r4 = std::make_shared<Rule5tpl>(3, 5000, 10000, 0xFFFF, 0xFFFF, 0, 1024, 111, 222, 0, 6);
  rules.push_back(r1);
  rules.push_back(r2);
  rules.push_back(r3);
  rules.push_back(r4);
  
  TrieNode n1(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  TrieNode n2(10001, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  TrieNode n3(10001, 0xFFFFFFFF, 0, 0xFFFFFFFF, 500, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  TrieNode n4(10000, 10000, 0xFFFF, 0xFFFF, 300, 1000, 201, 250, 4, 6);
  TrieNode n5(10000, 10000, 0xFFFF, 0xFFFF, 300, 1000, 201, 250, 10, 10);
 
  n1.setRules(rules);
  n2.setRules(rules);
  n3.setRules(rules);
  n4.setRules(rules);
  n5.setRules(rules);

  assert_equal(n1.getRuleAmount(), (unsigned)4, SPOT);
  assert_equal(n2.getRuleAmount(), (unsigned)2, SPOT);
  assert_equal(n3.getRuleAmount(), (unsigned)1, SPOT);
  assert_equal(n4.getRuleAmount(), (unsigned)1, SPOT);
  assert_equal(n5.getRuleAmount(), (unsigned)0, SPOT);
}

TEST(test_datahicuts_trienode_build)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  RulesVector rules;
  std::shared_ptr<Rule5tpl> r1 = std::make_shared<Rule5tpl>(0, 1, 2, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  std::shared_ptr<Rule5tpl> r2 = std::make_shared<Rule5tpl>(1, 3, 4, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  std::shared_ptr<Rule5tpl> r3 = std::make_shared<Rule5tpl>(2, 6, 7, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  std::shared_ptr<Rule5tpl> r4 = std::make_shared<Rule5tpl>(3, 8, 9, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  rules.push_back(r1);
  rules.push_back(r2);
  rules.push_back(r3);
  rules.push_back(r4);
  
  Parameters params(2, 2.0, 4);
  TrieNode n1(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
 
  n1.setRules(rules);
  try {
    n1.build(params);
    assert_equal(n1.getRuleAmount(), (unsigned)0, SPOT);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }
}

TEST(test_datahicuts_trienode_search)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  std::shared_ptr<Rule5tpl> r1 = std::make_shared<Rule5tpl>(0, 0, 20000, 0xFFFF, 0xFFFF, 80, 256, 128, 512, 6, 6);
  std::shared_ptr<Rule5tpl> r2 = std::make_shared<Rule5tpl>(1, 0, 10000, 0xFFFF, 0xFFFF, 100, 200, 42, 10000, 6, 12);
  std::shared_ptr<Rule5tpl> r3 = std::make_shared<Rule5tpl>(2, 0, 15000, 0xFFFF, 0xFFFF, 0, 1024, 100, 200, 6, 6);
  std::shared_ptr<Rule5tpl> r4 = std::make_shared<Rule5tpl>(3, 5000, 10000, 0xFFFF, 0xFFFF, 0, 1024, 111, 222, 0, 6);
  RulesVector rules1, rules2, rules3, rules4;
  rules1.push_back(r1);
  rules1.push_back(r2);
  rules1.push_back(r3);
  rules1.push_back(r4);

  rules2.push_back(r1);
  rules2.push_back(r3);

  rules3.push_back(r3);

  rules4.push_back(r3);
  rules4.push_back(r4);
  

  Parameters p1(1, 3.0, rules1.size());
  Parameters p2(2, 2.0, rules1.size());
  Parameters p3(4, 4.0, rules1.size());

  TrieNode n1(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  TrieNode n2(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  TrieNode n3(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  TrieNode n4(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
  TrieNode n5(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFF, 0, 0xFFFF, 0, 0xFF);
 
  Data5tpl::HeaderTuple hdr1(6000, 0xFFFF, 150, 150, 6); // matched by (r1, r2, r3, r4)
  Data5tpl::HeaderTuple hdr2(9999, 0xFFFF, 100, 5000, 7); // matched by (r2)
  Data5tpl::HeaderTuple hdr3(4999, 0xFFFF, 100, 100, 6); // matched by (r2, r3)
  Data5tpl::HeaderTuple hdr4(11111, 0xFFFF, 100, 164, 6); // matched by (r1, r3)
  Data5tpl::HeaderTuple hdr5(5000, 0xFFFF, 999, 199, 6); // matched by (r3, r4)
  Data5tpl::HeaderTuple hdr6(8080, 0xFFFF, 80, 222, 6); // matched by (r1, r4)
  Data5tpl::HeaderTuple hdr7(6042, 0xFFFF, 150, 101, 6); // matched by (r2, r3, r4)
  Data5tpl::HeaderTuple hdr8(10000, 0xFFFF, 1024, 200, 0); // matched by (r4)
  Data5tpl::HeaderTuple hdr9(30000, 0xFFFF, 100, 100, 6); // matched by none
  unsigned int matchIndex = 0;

  n1.setRules(rules1); // (r1, r2, r3, r4)
  n2.setRules(rules2); // (r1, r3)
  n3.setRules(rules3); // (r3)
  n4.setRules(rules1); // (r1, r2, r3, r4)
  n5.setRules(rules4); // (r3, r4)

  try {
    n1.build(p2); 
    n2.build(p1); // this one takes very long time
    n3.build(p3); 
    n4.build(p1);
    n5.build(p1);
  } catch (char const *ex) {
    assert_true(false, ex, SPOT);
  }
  
  // let each trie match all headers
  try {
    assert_true(n1.search(hdr1, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n1.search(hdr2, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)1, SPOT);
    assert_true(n1.search(hdr3, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)1, SPOT);
    assert_true(n1.search(hdr4, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n1.search(hdr5, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n1.search(hdr6, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n1.search(hdr7, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)1, SPOT);
    assert_true(n1.search(hdr8, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)3, SPOT);
    assert_false(n1.search(hdr9, matchIndex), SPOT);
  } catch (char const *ex) {
    assert_true(false, ex, SPOT);
  }
    
  try {
    assert_true(n2.search(hdr1, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_false(n2.search(hdr2, matchIndex), SPOT);
    assert_true(n2.search(hdr3, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n2.search(hdr4, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n2.search(hdr5, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n2.search(hdr6, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n2.search(hdr7, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_false(n2.search(hdr8, matchIndex), SPOT);
    assert_false(n2.search(hdr9, matchIndex), SPOT);
  } catch (char const *ex) {
    assert_true(false, ex, SPOT);
  }

  try {
    assert_true(n3.search(hdr1, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_false(n3.search(hdr2, matchIndex), SPOT);
    assert_true(n3.search(hdr3, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n3.search(hdr4, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n3.search(hdr5, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_false(n3.search(hdr6, matchIndex), SPOT);
    assert_true(n3.search(hdr7, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_false(n3.search(hdr8, matchIndex), SPOT);
    assert_false(n3.search(hdr9, matchIndex), SPOT);
  } catch (char const *ex) {
    assert_true(false, ex, SPOT);
  }

  try {
    assert_true(n4.search(hdr1, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n4.search(hdr2, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)1, SPOT);
    assert_true(n4.search(hdr3, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)1, SPOT);
    assert_true(n4.search(hdr4, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n4.search(hdr5, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n4.search(hdr6, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)0, SPOT);
    assert_true(n4.search(hdr7, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)1, SPOT);
    assert_true(n4.search(hdr8, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)3, SPOT);
    assert_false(n4.search(hdr9, matchIndex), SPOT);
  } catch (char const *ex) {
    assert_true(false, ex, SPOT);
  }

  try {
    assert_true(n5.search(hdr1, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_false(n5.search(hdr2, matchIndex), SPOT);
    assert_true(n5.search(hdr3, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n5.search(hdr4, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n5.search(hdr5, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n5.search(hdr6, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)3, SPOT);
    assert_true(n5.search(hdr7, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)2, SPOT);
    assert_true(n5.search(hdr8, matchIndex), SPOT);
    assert_equal(matchIndex, (unsigned)3, SPOT);
    assert_false(n5.search(hdr9, matchIndex), SPOT);
  } catch (char const *ex) {
    assert_true(false, ex, SPOT);
  }
}

