#include <test/AlgTestFixtures.hpp>
#include <memory>
#include <libunittest/all.hpp>

void AlgTestFixtures::fillRuleSetSmall(Generic::RuleSet& ruleset) {
  using namespace Generic;

  std::unique_ptr<Rule> rule1(new Rule);
  std::unique_ptr<RuleAtom> atom11(new RuleAtomExact((uint32_t)0xC2D27777));
  std::unique_ptr<RuleAtom> atom12(new RuleAtomExact((uint32_t)0xC2D2CCCC));
  std::unique_ptr<RuleAtom> atom13(new RuleAtomRange((uint16_t)99, (uint16_t)256));
  std::unique_ptr<RuleAtom> atom14(new RuleAtomExact((uint16_t)80));
  std::unique_ptr<RuleAtom> atom15(new RuleAtomExact((uint8_t)6));
  rule1->push_back(std::move(atom11));
  rule1->push_back(std::move(atom12));
  rule1->push_back(std::move(atom13));
  rule1->push_back(std::move(atom14));
  rule1->push_back(std::move(atom15));

  std::unique_ptr<Rule> rule2(new Rule);
  std::unique_ptr<RuleAtom> atom21(new RuleAtomExact((uint32_t)0xC2D27777));
  std::unique_ptr<RuleAtom> atom22(new RuleAtomPrefix((uint32_t)0xC2D2CCCC, (uint32_t)0xFFFFFF00));
  std::unique_ptr<RuleAtom> atom23(new RuleAtomRange((uint16_t)99, (uint16_t)256));
  std::unique_ptr<RuleAtom> atom24(new RuleAtomExact((uint16_t)80));
  std::unique_ptr<RuleAtom> atom25(new RuleAtomExact((uint8_t)6));
  rule2->push_back(std::move(atom21));
  rule2->push_back(std::move(atom22));
  rule2->push_back(std::move(atom23));
  rule2->push_back(std::move(atom24));
  rule2->push_back(std::move(atom25));

  ruleset.push_back(std::move(rule1));
  ruleset.push_back(std::move(rule2));
}

void AlgTestFixtures::_addHeaderLine1(Generic::PacketHeaderSet& packets, uint32_t addrDest, uint16_t portSrc) {
  using namespace Generic;

  std::unique_ptr<PacketHeaderLine> line(new PacketHeaderLine);
  std::unique_ptr<PacketHeaderAtom> atom1(new PacketHeaderAtom((uint32_t)0xC2D27777));
  std::unique_ptr<PacketHeaderAtom> atom2(new PacketHeaderAtom(addrDest));
  std::unique_ptr<PacketHeaderAtom> atom3(new PacketHeaderAtom((uint32_t)portSrc));
  std::unique_ptr<PacketHeaderAtom> atom4(new PacketHeaderAtom((uint32_t)80));
  std::unique_ptr<PacketHeaderAtom> atom5(new PacketHeaderAtom((uint32_t)6));
  line->push_back(std::move(atom1));
  line->push_back(std::move(atom2));
  line->push_back(std::move(atom3));
  line->push_back(std::move(atom4));
  line->push_back(std::move(atom5));
  packets.push_back(std::move(line));
}

void AlgTestFixtures::fillHeaderSet32(Generic::PacketHeaderSet& packets) {
  using namespace Generic;

  auto rndAccept = unittest::make_random_value<uint16_t>(99, 256);
  auto rndDrop = unittest::make_random_value<uint16_t>(257, 0xFFFF);
  rndAccept.seed(1024);
  rndDrop.seed(0xCAFE);

  uint32_t addrDest;
  uint16_t portSrc;
  for (int i = 0; i < 32; ++i) {
    if (i < 3 || (i > 8 && i < 11)) { // [0, 1, 2, 9, 10] -> rules[1]
      portSrc = rndAccept.get();
      addrDest = 0xC2D2CC66;
    } else if (i == 3 || i == 11 || i == 16) { // [3, 11, 16] -> rules[0]
      portSrc = 128;
      addrDest = 0xC2D2CCCC;
    }	else {
      portSrc = rndDrop.get();
      addrDest = (i%2==0 ? 0xC2D2CCCC : 0xABCD1234); // no matter which chosen: no match with any rule
    }

    _addHeaderLine1(packets, addrDest,  portSrc);
  }
}
  
void AlgTestFixtures::evalIndicesSet32(Generic::RuleIndexSet& indices) {
  using namespace unittest::assertions;

  assert_equal(indices.size(), (unsigned)32, SPOT);

  for (int i = 0; i < 32; ++i) {
    if (i < 3 || (i > 8 && i < 11)) // [0, 1, 2, 9, 10] -> rules[1]
      assert_equal(indices[i], (uint)1, SPOT);
    else if (i == 3 || i == 11 || i == 16) // [3, 11, 16] -> rules[0]
      assert_equal(indices[i], (uint)0, SPOT);
    else
      assert_equal(indices[i], Generic::noRuleIsMatching(), SPOT);
  }
}

void AlgTestFixtures::_addRule1(Generic::RuleSet& ruleset, uint16_t portSrc) {
  using namespace Generic;

  std::unique_ptr<Rule> rule(new Rule);
  std::unique_ptr<RuleAtom> atom1(new RuleAtomPrefix((uint32_t)0xC2D27777, (uint32_t)0xFFFFFF00));
  std::unique_ptr<RuleAtom> atom2(new RuleAtomExact((uint32_t)0xC2D2ABCD));
  std::unique_ptr<RuleAtom> atom3(new RuleAtomExact((uint16_t)portSrc));
  std::unique_ptr<RuleAtom> atom4(new RuleAtomRange((uint16_t)10000, (uint16_t)20000));
  std::unique_ptr<RuleAtom> atom5(new RuleAtomExact((uint8_t)6));
  rule->push_back(std::move(atom1));
  rule->push_back(std::move(atom2));
  rule->push_back(std::move(atom3));
  rule->push_back(std::move(atom4));
  rule->push_back(std::move(atom5));
  ruleset.push_back(std::move(rule));
}

void AlgTestFixtures::fillRuleSetBig(Generic::RuleSet& ruleset) {
  using namespace Generic;

  for (uint16_t i = 0; i < 32; ++i) {
    AlgTestFixtures::_addRule1(ruleset, i+1);
  }
}

void AlgTestFixtures::_addHeaderLine2(Generic::PacketHeaderSet& packets, uint32_t addrSrc, uint16_t portSrc) {
  using namespace Generic;

  std::unique_ptr<PacketHeaderLine> line(new PacketHeaderLine);
  std::unique_ptr<PacketHeaderAtom> atom1(new PacketHeaderAtom(addrSrc));
  std::unique_ptr<PacketHeaderAtom> atom2(new PacketHeaderAtom((uint32_t)0xC2D2ABCD));
  std::unique_ptr<PacketHeaderAtom> atom3(new PacketHeaderAtom((uint32_t)portSrc));
  std::unique_ptr<PacketHeaderAtom> atom4(new PacketHeaderAtom((uint32_t)11111));
  std::unique_ptr<PacketHeaderAtom> atom5(new PacketHeaderAtom((uint32_t)6));
  line->push_back(std::move(atom1));
  line->push_back(std::move(atom2));
  line->push_back(std::move(atom3));
  line->push_back(std::move(atom4));
  line->push_back(std::move(atom5));
  packets.push_back(std::move(line));
}

void AlgTestFixtures::fillHeaderSet1024(Generic::PacketHeaderSet& packets) {
  using namespace Generic;

  auto rndAddrSrc = unittest::make_random_value<uint32_t>(0xC2D27701, 0xC2D277FF);
  auto rndPortSrc = unittest::make_random_value<uint16_t>(1024, 0xFFFF);
  rndAddrSrc.seed(1024);
  rndPortSrc.seed(9690);

  uint16_t portSrc;
  for (uint16_t i = 0; i < 1024; ++i) {
    if (i%32 == 0) { // [0]->rules[0], [32]->rules[1], [64]->rules[2], [96]->rules[3], ...
      portSrc = i/32+1;
    }	else {
      portSrc = rndPortSrc.get();
    }

    AlgTestFixtures::_addHeaderLine2(packets, rndAddrSrc.get(),  portSrc);
  }
}

void AlgTestFixtures::evalIndicesSet1024(Generic::RuleIndexSet& indices) {
  using namespace unittest::assertions;

  assert_equal(indices.size(), (unsigned)1024, SPOT);
  
  uint32_t correctIndexCounter = 0;
  for (uint16_t i = 0; i < 1024; ++i) {
    if (i%32 == 0) {
      assert_equal(indices[i], correctIndexCounter++, SPOT);
    }
    else
      assert_equal(indices[i], Generic::noRuleIsMatching(), SPOT);
  }
}

void AlgTestFixtures::setupMemChrono(MemChronoSetup& setup) {
  using namespace Memory;
  setup.memMgrPtr = std::make_shared<MemManager>();
  setup.chrMgrPtr = std::make_shared<ChronoManager>();;
  setup.memRegPtr = std::make_shared<MemTraceRegistry>(setup.memMgrPtr);
  memRegistryPtr = setup.memRegPtr; // for registering MemTrace-instances
}


