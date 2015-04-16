#ifndef ALG_TEST_FIXTURES_INCLUDED
#define ALG_TEST_FIXTURES_INCLUDED

#include <data/RuleSet.hpp>
#include <data/PacketHeader.hpp>
#include <cstdint>
#include <memory>
#include <memory/MemManager.hpp>
#include <memory/MemTrace.hpp>
#include <memory/MemTraceRegistry.hpp>
#include <core/ChronoManager.hpp>

/** For holding the MemRegistry and MemManager instances. */
struct MemChronoSetup {
  std::shared_ptr<Memory::MemManager> memMgrPtr;
  std::shared_ptr<ChronoManager> chrMgrPtr;
  std::shared_ptr<Memory::MemTraceRegistry> memRegPtr;
};

/** Provides some fixtures for testing algorithms in unit tests. */
class AlgTestFixtures {
  
  /** Adds a packet header line to a header set with variable addr. and port. */
  static void _addHeaderLine1(Generic::PacketHeaderSet& packets, uint32_t addrDest, uint16_t portSrc);

  /** Adds a packet header line to a header set with variable addr. and port. */
  static void _addHeaderLine2(Generic::PacketHeaderSet& packets, uint32_t addrSrc, uint16_t portSrc);

  /** Adds a rule to a rule set with variable source-port. */
  static void _addRule1(Generic::RuleSet& ruleset, uint16_t portSrc);

public:

  AlgTestFixtures() = delete;

  /** Create a small rule set with only two rules */
  static void fillRuleSetSmall(Generic::RuleSet& ruleset);

  /** Create 32 headers and fill them into given set. */
  static void fillHeaderSet32(Generic::PacketHeaderSet& packets);

  /** Evaluates the 32 indices after a classification run. */
  static void evalIndicesSet32(Generic::RuleIndexSet& indices);

  /** Creates a big rule set with 32 rules. */
  static void fillRuleSetBig(Generic::RuleSet& ruleset);

  /** Create 1024 headers and fill them into given set. */
  static void fillHeaderSet1024(Generic::PacketHeaderSet& packets);

  /** Evaluates the 1024 indices after a classification run. */
  static void evalIndicesSet1024(Generic::RuleIndexSet& indices);

  /** Creates and setups necessary Memory and Chronograph instances. */
  static void setupMemChrono(MemChronoSetup& setup);
};

#endif

