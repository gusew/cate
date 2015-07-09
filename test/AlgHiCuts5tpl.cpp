#include <libunittest/all.hpp>
#include <test/AlgTestFixtures.hpp>
#include <algorithms/hicuts/HiCuts5tpl.hpp>
#include <memory>

using namespace unittest::assertions;
using namespace Memory;

TEST(test_alg_hicuts_5tpl_classify)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Generic;

  RuleSet ruleset;
  AlgTestFixtures::fillRuleSetSmall(ruleset);
  assert_true(ruleSetIsValid(ruleset), SPOT);

  PacketHeaderSet packets;
  AlgTestFixtures::fillHeaderSet32(packets);
  assert_equal(packets.size(), (uint)32, SPOT);
  
  RuleIndexSet indices;
  assert_true(indices.empty(), SPOT);
  
  std::vector<double> params;
  params.push_back(100.0);
  params.push_back(1.0); // binth
  params.push_back(1.5); // spfac

  std::unique_ptr<HiCuts5tpl> alg(new HiCuts5tpl);
  try {
    alg->setMemManager(setup.memMgrPtr);
    alg->setChronoManager(setup.chrMgrPtr);
    alg->setParameters(params);
    alg->setRules(ruleset);
    alg->classify(packets, indices);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }
  
  AlgTestFixtures::evalIndicesSet32(indices);
}

TEST(test_alg_hicuts_5tpl_classify_big)
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
  
  std::vector<double> params;
  params.push_back(100.0);
  params.push_back(4.0); // binth
  params.push_back(5.0); // spfac

  std::unique_ptr<HiCuts5tpl> alg(new HiCuts5tpl);
  try {
    alg->setMemManager(setup.memMgrPtr);
    alg->setChronoManager(setup.chrMgrPtr);
    alg->setParameters(params);
    alg->setRules(ruleset);
    alg->classify(packets, indices);
  } catch (char const* ex) {
    assert_true(false, ex, SPOT);
  }

  AlgTestFixtures::evalIndicesSet1024(indices);
}

TEST(test_alg_hicuts_5tpl_ruleadd_empty)
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

  std::vector<double> params;
  params.push_back(100.0);
  params.push_back(1.0); // binth
  params.push_back(5.0); // spfac

  HiCuts5tpl alg;
  try {
    alg.setMemManager(setup.memMgrPtr);
    alg.setChronoManager(setup.chrMgrPtr);
    alg.setParameters(params);
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

TEST(test_alg_hicuts_5tpl_ruleadd_filled)
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
 
  std::vector<double> params;
  params.push_back(100.0);
  params.push_back(3.0); // binth
  params.push_back(2.5); // spfac

  HiCuts5tpl alg;
  try {
    alg.setMemManager(setup.memMgrPtr);
    alg.setChronoManager(setup.chrMgrPtr);
    alg.setParameters(params);
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

TEST(test_alg_hicuts_5tpl_ruleremoved)
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
 
  std::vector<double> params;
  params.push_back(100.0);
  params.push_back(1.0); // binth
  params.push_back(3.0); // spfac

  HiCuts5tpl alg;
  try {
    alg.setMemManager(setup.memMgrPtr);
    alg.setChronoManager(setup.chrMgrPtr);
    alg.setParameters(params);
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

