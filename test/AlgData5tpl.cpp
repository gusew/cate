#include <libunittest/all.hpp>
#include <data/RuleSet.hpp>
#include <data/PacketHeader.hpp>
#include <algorithms/common/Data5tpl.hpp>
#include <memory>
#include <test/AlgTestFixtures.hpp>

using namespace unittest::assertions;
using namespace Memory;

/** Low level: Test each rule atom type for correctness */
TEST(test_algdata_5tpl_ruleatom_exact)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Data5tpl::RuleAtomExact<uint16_t> exact16(44042);
  assert_true(exact16.match(44042), SPOT);
  assert_false(exact16.match(321), SPOT);

  Data5tpl::RuleAtomExact<uint32_t> exact32(0x12349876);
  assert_true(exact32.match(0x12349876), SPOT);
  assert_false(exact32.match(0x55555555), SPOT);
}

TEST(test_algdata_5tpl_ruleatom_range)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Data5tpl::RuleAtomRange<uint8_t> range8(0x64, 0x88);
  assert_true(range8.match(0x64), SPOT);
  assert_true(range8.match(0x72), SPOT);
  assert_true(range8.match(0x88), SPOT);
  assert_false(range8.match(0x63), SPOT);
  assert_false(range8.match(0x89), SPOT);
  assert_false(range8.match(0), SPOT);
  assert_false(range8.match(0xFF), SPOT);

  Data5tpl::RuleAtomRange<uint32_t> range32(0xABCD0064, 0xABCD00FF);
  assert_true(range32.match(0xABCD0064), SPOT);
  assert_true(range32.match(0xABCD0099), SPOT);
  assert_true(range32.match(0xABCD00FF), SPOT);
  assert_false(range32.match(0xABCD0063), SPOT);
  assert_false(range32.match(0xABCD0100), SPOT);
  assert_false(range32.match(0), SPOT);
  assert_false(range32.match(0xFFFFFFFF), SPOT);
}

TEST(test_algdata_5tpl_ruleatom_prefix)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Data5tpl::RuleAtomPrefix<uint8_t> prefix8(0x2D, 0xFC);
  assert_true(prefix8.match(0x2C), SPOT);
  assert_true(prefix8.match(0x2D), SPOT);
  assert_true(prefix8.match(0x2E), SPOT);
  assert_true(prefix8.match(0x2F), SPOT);
  assert_false(prefix8.match(0x2A), SPOT);
  assert_false(prefix8.match(0xF), SPOT);
  assert_false(prefix8.match(0xff), SPOT);
  assert_false(prefix8.match(0), SPOT);

  Data5tpl::RuleAtomPrefix<uint16_t> prefix16(0x14E0, 0xFFE0);
  assert_true(prefix16.match(0x14E0), SPOT);
  assert_true(prefix16.match(0x14F3), SPOT);
  assert_true(prefix16.match(0x14FF), SPOT);
  assert_false(prefix16.match(0x14D0), SPOT);
  assert_false(prefix16.match(0x34E0), SPOT);
  assert_false(prefix16.match(0), SPOT);
  assert_false(prefix16.match(0xffff), SPOT);

  Data5tpl::RuleAtomPrefix<uint32_t> prefix32(0x2AAAAAAA, 0xFFFFFFE);
  assert_true(prefix32.match(0x2AAAAAAA), SPOT);
  assert_true(prefix32.match(0x2AAAAAAB), SPOT);
  assert_false(prefix32.match(0x2AAAAAAF), SPOT);
  assert_false(prefix32.match(0), SPOT);
  assert_false(prefix32.match(0xFFFFFFFF), SPOT);

  Data5tpl::RuleAtomPrefix<uint64_t> prefix64(0xABCDABCD12345678, 0xFFFFFFFF00000000);
  assert_true(prefix64.match(0xABCDABCD12345678), SPOT);
  assert_true(prefix64.match(0xABCDABCD00000000), SPOT);
  assert_true(prefix64.match(0xABCDABCDFFFFFFFF), SPOT);
  assert_false(prefix64.match(0xABCDABCEFFFFFFFF), SPOT);
  assert_false(prefix64.match(0x1BCDABCDFFFFFFFF), SPOT);
  assert_false(prefix64.match(0), SPOT);
  assert_false(prefix64.match(0xFFFFFFFFFFFFFFFF), SPOT);
}

/** Low level: Test specific rule type for correctness */
TEST(test_algdata_5tpl_rule)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Data5tpl;

  std::unique_ptr<RuleAtomExact<uint32_t>> addrSrc(new RuleAtomExact<uint32_t>(0xC0A8DE20));
  std::unique_ptr<RuleAtomPrefix<uint32_t>> addrDest(new RuleAtomPrefix<uint32_t>(0xC0A80000, 0xFFFF0000));
  std::unique_ptr<RuleAtomRange<uint16_t>> portSrc(new RuleAtomRange<uint16_t>(1, 0xFFFF));
  std::unique_ptr<RuleAtomExact<uint16_t>> portDest(new RuleAtomExact<uint16_t>(80));
  std::unique_ptr<RuleAtomExact<uint8_t>> protocol(new RuleAtomExact<uint8_t>(6));

  Rule rule(std::move(addrSrc), std::move(addrDest), std::move(portSrc), std::move(portDest), std::move(protocol));

  HeaderTuple tuple1(0xC0A8DE20, 0xC0A80000, 1, 80, 6);
  assert_true(rule.match(tuple1), SPOT);
  HeaderTuple tuple2(0xC0A8DE20, 0xC0A8FFFF, 0xFFFF, 80, 6);
  assert_true(rule.match(tuple1), SPOT);
  HeaderTuple tuple3(0xC0A8DE20, 0xC0A81234, 0x1234, 80, 6);
  assert_true(rule.match(tuple3), SPOT);

  HeaderTuple tuple4(0xC0A8DE21, 0xC0A80000, 1, 80, 6);
  assert_false(rule.match(tuple4), SPOT);
  HeaderTuple tuple5(0xC0A8DE20, 0xC0A70000, 0xFFFF, 80, 6);
  assert_false(rule.match(tuple5), SPOT);
  HeaderTuple tuple6(0xC0A8DE20, 0xD0A8FFFF, 0x1234, 80, 6);
  assert_false(rule.match(tuple6), SPOT);

  HeaderTuple tuple7(0xC0A8DE20, 0xC0A8DE20, 0, 80, 6);
  assert_false(rule.match(tuple7), SPOT);
  HeaderTuple tuple8(0xC0A8DE20, 0xC0A8DE20, 0xFF, 8080, 6);
  assert_false(rule.match(tuple8), SPOT);
  HeaderTuple tuple9(0xC0A8DE20, 0xC0A8DE20, 0xFFFF, 80, 7);
  assert_false(rule.match(tuple9), SPOT);

  HeaderTuple tuple10(0xC0A8DE20, 0xC0A8DE20, 80, 80, 5);
  assert_false(rule.match(tuple10), SPOT);
}

/** Medium level: Test rule set for correctness */
TEST(test_algdata_5tpl_ruleset)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  using namespace Data5tpl;

  // simple setup: two rules, several tuples
  RuleSet ruleset(2);

  std::unique_ptr<RuleAtomExact<uint32_t>> addrSrc1(new RuleAtomExact<uint32_t>(0xC2D25511));
  std::unique_ptr<RuleAtomPrefix<uint32_t>> addrDest1(new RuleAtomPrefix<uint32_t>(0xC2D25511, 0xFFFF0000));
  std::unique_ptr<RuleAtomRange<uint16_t>> portSrc1(new RuleAtomRange<uint16_t>(1, 0xFFFF));
  std::unique_ptr<RuleAtomExact<uint16_t>> portDest1(new RuleAtomExact<uint16_t>(80));
  std::unique_ptr<RuleAtomExact<uint8_t>> protocol1(new RuleAtomExact<uint8_t>(6));
  std::unique_ptr<Rule> rule1(new Rule(std::move(addrSrc1), std::move(addrDest1), std::move(portSrc1), std::move(portDest1), std::move(protocol1)));

  std::unique_ptr<RuleAtomPrefix<uint32_t>> addrSrc2(new RuleAtomPrefix<uint32_t>(0xC2D21100, 0xFFFFFF00));
  std::unique_ptr<RuleAtomRange<uint32_t>> addrDest2(new RuleAtomRange<uint32_t>(0xC2D21101, 0xC2D211FF));
  std::unique_ptr<RuleAtomRange<uint16_t>> portSrc2(new RuleAtomRange<uint16_t>(1, 0xFFFF));
  std::unique_ptr<RuleAtomExact<uint16_t>> portDest2(new RuleAtomExact<uint16_t>(22));
  std::unique_ptr<RuleAtomExact<uint8_t>> protocol2(new RuleAtomExact<uint8_t>(6));
  std::unique_ptr<Rule> rule2(new Rule(std::move(addrSrc2), std::move(addrDest2), std::move(portSrc2), std::move(portDest2), std::move(protocol2)));

  ruleset.setRule(0, std::move(rule1));
  ruleset.setRule(1, std::move(rule2));

  uint32_t matched = 0xFFFFFFFF;

  HeaderTuple tuple1(0xC2D25511, 0xC2D2ABCD, 20000, 80, 6);
  assert_true(ruleset.match(tuple1, matched), SPOT);
  assert_equal(matched, (unsigned)0, SPOT);

  HeaderTuple tuple2(0xC2D21142, 0xC2D21133, 22222, 22, 6);
  assert_true(ruleset.match(tuple2, matched), SPOT);
  assert_equal(matched, (unsigned)1, SPOT);

  HeaderTuple tuple3(0x12345678, 0xC2D2ABCD, 20000, 80, 6);
  assert_false(ruleset.match(tuple3, matched), SPOT);

  HeaderTuple tuple4(0xC2D21142, 0xC2D21133, 22222, 42, 6);
  assert_false(ruleset.match(tuple4, matched), SPOT);

  HeaderTuple tuple5(0, 0, 0, 0, 0);
  assert_false(ruleset.match(tuple5, matched), SPOT);
}

