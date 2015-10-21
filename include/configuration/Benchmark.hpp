#ifndef BENCHMARK_INCLUDED
#define BENCHMARK_INCLUDED

#include <vector>
#include <memory>
#include <string>
#include <generics/RuleSet.hpp>
#include <generics/PacketHeader.hpp>
#include <configuration/RandomHeaderConfiguration.hpp>

/** contains parameters to configure an algorithm */
typedef std::vector<double> AlgorithmParameterSet;

/** contains the structure of header (and rule set) fields in bits per field */
typedef std::vector<unsigned int> FieldStructureSet;

/**
 * Represents one benchmark with a certain algorithm, a ruleset and
 * headers as input. A benchmark is repeated, if number of runs is 
 * greater than one.
 */
struct Benchmark {
  /** Unique identifier (will be set once by the configuration). */
  std::string id;
  /** Short description of benchmark. */
  std::string caption;

  std::string algFilename;
  AlgorithmParameterSet algParameter;

  FieldStructureSet fieldStructure;
  
  bool generateHeaders;
  Generic::PacketHeaderSet headers;
  RandomHeaderConfiguration rndHeaderConfig;
  
  bool generateRules;
  Generic::RuleSet rules;
  // RandomRuleConfiguration rndRuleConfig;

  /** Defines how often the benchmark will be run. */
  unsigned int numberRuns;

  Benchmark() : algParameter(), fieldStructure(), generateHeaders(false), headers(), rndHeaderConfig(), generateRules(false), rules(), numberRuns(1) {}

  /** Returns the total number of headers (random or explicit). */
  inline unsigned int getHeaderNumber() const { return (generateHeaders ? rndHeaderConfig.totalHeaders : headers.size()); }

  /** Returns the total number of rules (now: just explicit). */
  inline unsigned int getRuleNumber() const { return (generateRules ? 0 : rules.size()); }
};

/** Shortcut for a shared-ptr on a Benchmark-instance. */
typedef std::shared_ptr<Benchmark> BenchmarkPtr;

#endif

