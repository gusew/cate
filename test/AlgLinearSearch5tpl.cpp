#include <libunittest/all.hpp>
#include <test/AlgTestFixtures.hpp>
#include <algorithms/common/Data5tpl.hpp>
#include <algorithms/linear/LinearSearch5tpl.hpp>
#include <memory>

using namespace unittest::assertions;
using namespace Memory;

TEST(test_alglinsearch_5tpl_classify)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Generic;

  RuleSet ruleset;
  AlgTestFixtures::fillRuleSetSmall(ruleset);
  assert_true(ruleSetIsValid(ruleset), SPOT);

  PacketHeaderSet packets;
  AlgTestFixtures::fillHeaderSet32(packets);
  assert_equal(packets.size(), (unsigned)32, SPOT);
  
  RuleIndexSet indices;
  assert_true(indices.empty(), SPOT);
  
  std::unique_ptr<LinearSearch5tpl> alg(new LinearSearch5tpl);
  try {
    alg->setMemManager(setup.memMgrPtr);
    alg->setChronoManager(setup.chrMgrPtr);
    alg->setRules(ruleset);
    alg->classify(packets, indices);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }
 
  AlgTestFixtures::evalIndicesSet32(indices);
}

TEST(test_alglinsearch_5tpl_classify_big)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Generic;

  RuleSet ruleset;
  AlgTestFixtures::fillRuleSetBig(ruleset);
  assert_true(ruleSetIsValid(ruleset), SPOT);

  PacketHeaderSet packets;
  AlgTestFixtures::fillHeaderSet1024(packets);
  assert_equal(packets.size(), (unsigned int)1024, SPOT);
  
  RuleIndexSet indices;
  assert_true(indices.empty(), SPOT);
  
  std::unique_ptr<LinearSearch5tpl> alg(new LinearSearch5tpl);
  try {
    alg->setMemManager(setup.memMgrPtr);
    alg->setChronoManager(setup.chrMgrPtr);
    alg->setRules(ruleset);
    alg->classify(packets, indices);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }

  AlgTestFixtures::evalIndicesSet1024(indices);
}

TEST(test_algtuplespace_5tpl_classify_wc)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Generic;

  RuleSet ruleset;

  std::unique_ptr<Rule> rule1(new Rule);
  std::unique_ptr<RuleAtom> atom11(new RuleAtomPrefix((uint32_t)0, (uint32_t)0));
  std::unique_ptr<RuleAtom> atom12(new RuleAtomPrefix((uint32_t)0, (uint32_t)0));
  std::unique_ptr<RuleAtom> atom13(new RuleAtomRange((uint16_t)0, (uint16_t)0xFFFF));
  std::unique_ptr<RuleAtom> atom14(new RuleAtomRange((uint16_t)0, (uint16_t)0xFFFF));
  std::unique_ptr<RuleAtom> atom15(new RuleAtomExact((uint8_t)6));
  rule1->push_back(std::move(atom11));
  rule1->push_back(std::move(atom12));
  rule1->push_back(std::move(atom13));
  rule1->push_back(std::move(atom14));
  rule1->push_back(std::move(atom15));

  ruleset.push_back(std::move(rule1));
  assert_true(ruleSetIsValid(ruleset), SPOT);

  PacketHeaderSet packets;

  std::unique_ptr<PacketHeaderLine> line1(new PacketHeaderLine);
  std::unique_ptr<PacketHeaderAtom> hdratom11(new PacketHeaderAtom((uint32_t)4294967295));
  std::unique_ptr<PacketHeaderAtom> hdratom12(new PacketHeaderAtom((uint32_t)0));
  std::unique_ptr<PacketHeaderAtom> hdratom13(new PacketHeaderAtom((uint32_t)65535));
  std::unique_ptr<PacketHeaderAtom> hdratom14(new PacketHeaderAtom((uint32_t)0));
  std::unique_ptr<PacketHeaderAtom> hdratom15(new PacketHeaderAtom((uint32_t)6));
  line1->push_back(std::move(hdratom11));
  line1->push_back(std::move(hdratom12));
  line1->push_back(std::move(hdratom13));
  line1->push_back(std::move(hdratom14));
  line1->push_back(std::move(hdratom15));
  packets.push_back(std::move(line1));

  std::unique_ptr<PacketHeaderLine> line2(new PacketHeaderLine);
  std::unique_ptr<PacketHeaderAtom> hdratom21(new PacketHeaderAtom((uint32_t)0));
  std::unique_ptr<PacketHeaderAtom> hdratom22(new PacketHeaderAtom((uint32_t)4294967295));
  std::unique_ptr<PacketHeaderAtom> hdratom23(new PacketHeaderAtom((uint32_t)65535));
  std::unique_ptr<PacketHeaderAtom> hdratom24(new PacketHeaderAtom((uint32_t)65535));
  std::unique_ptr<PacketHeaderAtom> hdratom25(new PacketHeaderAtom((uint32_t)6));
  line2->push_back(std::move(hdratom21));
  line2->push_back(std::move(hdratom22));
  line2->push_back(std::move(hdratom23));
  line2->push_back(std::move(hdratom24));
  line2->push_back(std::move(hdratom25));
  packets.push_back(std::move(line2));

  assert_equal(packets.size(), (unsigned)2, SPOT);
  
  RuleIndexSet indices;
  assert_true(indices.empty(), SPOT);

  std::vector<double> params;
  params.push_back(1);

  std::unique_ptr<LinearSearch5tpl> alg(new LinearSearch5tpl);
  try {
    alg->setMemManager(setup.memMgrPtr);
    alg->setChronoManager(setup.chrMgrPtr);
    alg->setParameters(params);
    alg->setRules(ruleset);
    alg->classify(packets, indices);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }
 
  assert_equal(indices.size(), (unsigned)2, SPOT);
  assert_equal(indices[0], (unsigned)0, SPOT);
  assert_equal(indices[1], (unsigned)0, SPOT);
}

TEST(test_alglinsearch_5tpl_ruleadd_empty)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Generic;

  RuleSet ruleset;
  PacketHeaderSet packets;
  RuleIndexSet indices;
  
  // one header to test matching
  std::unique_ptr<PacketHeaderLine> hdrLine(new PacketHeaderLine);
  std::unique_ptr<PacketHeaderAtom> hdrAtom1(new PacketHeaderAtom((uint32_t)0x12345678)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom2(new PacketHeaderAtom((uint32_t)0x87654321)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom3(new PacketHeaderAtom((uint32_t)1024)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom4(new PacketHeaderAtom((uint32_t)32000)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom5(new PacketHeaderAtom((uint32_t)8)); 
  hdrLine->push_back(std::move(hdrAtom1));
  hdrLine->push_back(std::move(hdrAtom2));
  hdrLine->push_back(std::move(hdrAtom3));
  hdrLine->push_back(std::move(hdrAtom4));
  hdrLine->push_back(std::move(hdrAtom5));
  packets.push_back(std::move(hdrLine));

  LinearSearch5tpl alg;
  try {
    alg.setMemManager(setup.memMgrPtr);
    alg.setChronoManager(setup.chrMgrPtr);
    alg.setRules(ruleset); // empty ruleset
    alg.classify(packets, indices);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }

  assert_equal(indices.size(), (unsigned)1, SPOT);
  assert_equal(indices[0], noRuleIsMatching(), SPOT);

  // now add a matching rule
  Rule rule1;
  std::unique_ptr<RuleAtomExact> atom1(new RuleAtomExact((uint32_t)0x12345678));
  std::unique_ptr<RuleAtomExact> atom2(new RuleAtomExact((uint32_t)0x87654321));
  std::unique_ptr<RuleAtomExact> atom3(new RuleAtomExact((uint32_t)1024));
  std::unique_ptr<RuleAtomExact> atom4(new RuleAtomExact((uint32_t)32000));
  std::unique_ptr<RuleAtomExact> atom5(new RuleAtomExact((uint32_t)8));
  rule1.push_back(std::move(atom1));
  rule1.push_back(std::move(atom2));
  rule1.push_back(std::move(atom3));
  rule1.push_back(std::move(atom4));
  rule1.push_back(std::move(atom5));

  try {
    alg.ruleAdded(0, rule1);
    alg.classify(packets, indices);
  } catch(const char* ex) {
    assert_true(false, ex, SPOT);
  }

  assert_equal(indices.size(), (unsigned)1, SPOT);
  assert_equal(indices[0], (unsigned)0, SPOT);

  // now insert a non-matching rule 
  Rule rule2;
  std::unique_ptr<RuleAtomExact> atom21(new RuleAtomExact((uint32_t)0));
  std::unique_ptr<RuleAtomExact> atom22(new RuleAtomExact((uint32_t)0x87654321));
  std::unique_ptr<RuleAtomExact> atom23(new RuleAtomExact((uint32_t)1024));
  std::unique_ptr<RuleAtomExact> atom24(new RuleAtomExact((uint32_t)32000));
  std::unique_ptr<RuleAtomExact> atom25(new RuleAtomExact((uint32_t)8));
  rule2.push_back(std::move(atom21));
  rule2.push_back(std::move(atom22));
  rule2.push_back(std::move(atom23));
  rule2.push_back(std::move(atom24));
  rule2.push_back(std::move(atom25));

  try {
    alg.ruleAdded(0, rule2);
    alg.classify(packets, indices);
  } catch(const char* ex) {
    assert_true(false, ex, SPOT);
  }

  assert_equal(indices.size(), (unsigned)1, SPOT);
  assert_equal(indices[0], (unsigned)1, SPOT);
}

TEST(test_alglinsearch_5tpl_ruleadd_filled)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Generic;

  RuleSet rulesetBig;
  AlgTestFixtures::fillRuleSetBig(rulesetBig);

  PacketHeaderSet packets;
  AlgTestFixtures::fillHeaderSet1024(packets);
  
  RuleIndexSet indices;
 
  // add wildcard rule at position 8 (index = 7)
  Rule rule;
  std::unique_ptr<RuleAtomPrefix> atom1(new RuleAtomPrefix((uint32_t)0x12345678, (uint32_t)0));
  std::unique_ptr<RuleAtomPrefix> atom2(new RuleAtomPrefix((uint32_t)0x12345678, (uint32_t)0));
  std::unique_ptr<RuleAtomRange> atom3(new RuleAtomRange((uint32_t)0, (uint32_t)0xFFFF));
  std::unique_ptr<RuleAtomRange> atom4(new RuleAtomRange((uint32_t)0, (uint32_t)0xFFFF));
  std::unique_ptr<RuleAtomRange> atom5(new RuleAtomRange((uint32_t)0, (uint32_t)0xFF));
  rule.push_back(std::move(atom1));
  rule.push_back(std::move(atom2));
  rule.push_back(std::move(atom3));
  rule.push_back(std::move(atom4));
  rule.push_back(std::move(atom5));

  LinearSearch5tpl alg;
  try {
    alg.setMemManager(setup.memMgrPtr);
    alg.setChronoManager(setup.chrMgrPtr);
    alg.setRules(rulesetBig);
    alg.ruleAdded(7, rule);
    alg.classify(packets, indices);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }
  assert_equal(indices.size(), (unsigned)1024, SPOT);

  uint32_t correctIndexCounter = 0;
  for (uint16_t i = 0; i < 1024; ++i) {
    if (i%32 == 0 && correctIndexCounter < 8) 
      assert_equal(indices[i], correctIndexCounter++, SPOT);
    else
      assert_equal(indices[i], (unsigned)7, SPOT);
  }
}

TEST(test_alglinsearch_5tpl_ruleremoved)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Generic;

  RuleSet ruleset;
  AlgTestFixtures::fillRuleSetSmall(ruleset);

  PacketHeaderSet packets;
  // first header matches first rule
  std::unique_ptr<PacketHeaderLine> hdrLine1(new PacketHeaderLine);
  std::unique_ptr<PacketHeaderAtom> hdrAtom11(new PacketHeaderAtom((uint32_t)0xC2D27777)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom12(new PacketHeaderAtom((uint32_t)0xC2D2CCCC)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom13(new PacketHeaderAtom((uint32_t)128)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom14(new PacketHeaderAtom((uint32_t)80)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom15(new PacketHeaderAtom((uint32_t)6)); 
  hdrLine1->push_back(std::move(hdrAtom11));
  hdrLine1->push_back(std::move(hdrAtom12));
  hdrLine1->push_back(std::move(hdrAtom13));
  hdrLine1->push_back(std::move(hdrAtom14));
  hdrLine1->push_back(std::move(hdrAtom15));
  packets.push_back(std::move(hdrLine1));

  // second header matches second rule
  std::unique_ptr<PacketHeaderLine> hdrLine2(new PacketHeaderLine);
  std::unique_ptr<PacketHeaderAtom> hdrAtom21(new PacketHeaderAtom((uint32_t)0xC2D27777)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom22(new PacketHeaderAtom((uint32_t)0xC2D2CC88)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom23(new PacketHeaderAtom((uint32_t)256)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom24(new PacketHeaderAtom((uint32_t)80)); 
  std::unique_ptr<PacketHeaderAtom> hdrAtom25(new PacketHeaderAtom((uint32_t)6)); 
  hdrLine2->push_back(std::move(hdrAtom21));
  hdrLine2->push_back(std::move(hdrAtom22));
  hdrLine2->push_back(std::move(hdrAtom23));
  hdrLine2->push_back(std::move(hdrAtom24));
  hdrLine2->push_back(std::move(hdrAtom25));
  packets.push_back(std::move(hdrLine2));

  RuleIndexSet indices1;
  RuleIndexSet indices2;
  RuleIndexSet indices3;
 
  LinearSearch5tpl alg;
  try {
    alg.setMemManager(setup.memMgrPtr);
    alg.setChronoManager(setup.chrMgrPtr);
    alg.setRules(ruleset);
    alg.classify(packets, indices1);
    alg.ruleRemoved(0);
    alg.classify(packets, indices2);
    alg.ruleRemoved(0);
    alg.classify(packets, indices3);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }
  assert_equal(indices1.size(), (unsigned)2, SPOT);
  assert_equal(indices1[0], (unsigned)0, SPOT);
  assert_equal(indices1[1], (unsigned)1, SPOT);

  assert_equal(indices2.size(), (unsigned)2, SPOT);
  assert_equal(indices2[0], (unsigned)0, SPOT);
  assert_equal(indices2[1], (unsigned)0, SPOT);

  assert_equal(indices3.size(), (unsigned)2, SPOT);
  assert_equal(indices3[0], noRuleIsMatching(), SPOT);
  assert_equal(indices3[1], noRuleIsMatching(), SPOT);
}

