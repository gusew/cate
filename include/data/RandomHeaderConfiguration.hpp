#ifndef RANDOMHEADERCONFIGURATION_INCLUDED
#define RANDOMHEADERCONFIGURATION_INCLUDED

#include <vector>
#include <memory>
#include <data/RandomDistribution.hpp>

/** Type of a vector with all configured random distributions per header field. */
typedef std::vector<std::unique_ptr<RandomDistribution>> RandomDistributionSet;

/** Type of a vector with seed values for each random generator. */
typedef std::vector<unsigned int> RandomGeneratorSeedSet;

/** Holds the configuration of a header field random generator. */
struct RandomHeaderConfiguration {
  /** Total amount of packet headers to generate. */
  unsigned int totalHeaders;

  /** If true, generated headers will be output to a file in results directory. */
  bool outputToFile;

  /** All configured random distributions per header field. */
  RandomDistributionSet distributions;

  /** Seed values for each random generator. */
  RandomGeneratorSeedSet seeds;

  RandomHeaderConfiguration() : totalHeaders(0), outputToFile(false), distributions(), seeds() {}
};

#endif

